package com.xunews.backend.rag;

import com.xunews.backend.news.Article;
import com.xunews.backend.news.ArticleRepository;
import com.xunews.backend.news.ArticleService;
import com.xunews.backend.news.Chunk;
import com.xunews.backend.news.ChunkRepository;
import com.xunews.backend.rag.dto.IngestTextRequest;
import com.xunews.backend.search.BaiduClient;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;

@Service
@RequiredArgsConstructor
public class RagService {

    private final ArticleService articleService;
    private final ArticleRepository articleRepository;
    private final ChunkRepository chunkRepository;
    private final RagClient ragClient;
    private final BaiduClient baiduClient;

    @Transactional
    public Map<String, Object> ingestText(IngestTextRequest req) {
        // 1. Save article metadata/content
        Article article = articleService.createOrGetArticle(
                req.getSourceName(),
                req.getSourceUrl(),
                req.getTitle(),
                req.getUrl(),
                req.getContent()
        );

        // 2. Split content into chunks
        List<String> chunks = splitText(req.getContent(), 500);

        // 3. Prepare items for vector store with deterministic IDs
        List<Map<String, Object>> items = new ArrayList<>();
        List<Chunk> chunkEntities = new ArrayList<>();
        for (int i = 0; i < chunks.size(); i++) {
            String text = chunks.get(i);
            String vid = UUID.randomUUID().toString();
            Map<String, Object> meta = new HashMap<>();
            meta.put("article_id", article.getId());
            meta.put("title", article.getTitle());
            meta.put("source_name", article.getSource() != null ? article.getSource().getName() : null);
            meta.put("chunk_index", i);
            items.add(Map.of(
                    "text", text,
                    "metadata", meta,
                    "id", vid
            ));

            Chunk c = Chunk.builder()
                    .article(article)
                    .text(text)
                    .vectorId(vid)
                    .build();
            chunkEntities.add(c);
        }
        chunkRepository.saveAll(chunkEntities);

        // 4. Upsert to vector store
        int inserted = ragClient.upsert(items);

        return Map.of(
                "article_id", article.getId(),
                "chunks", chunks.size(),
                "inserted", inserted
        );
    }

    public List<Map<String, Object>> search(String query, int topK) {
        List<Map<String, Object>> hits = ragClient.search(query, topK);
        if (hits != null && !hits.isEmpty()) return hits;

        // Fallback: Baidu search top3 and summarize
        List<Map<String, String>> web = baiduClient.topResults(query, Math.min(3, topK));
        if (web.isEmpty()) return List.of();
        List<String> snippets = new ArrayList<>();
        for (Map<String, String> r : web) {
            String title = r.getOrDefault("title", "");
            String url = r.getOrDefault("url", "");
            snippets.add(title + " " + url);
        }
        String summary = ragClient.summarize(query, snippets);
        Map<String, Object> summaryItem = new HashMap<>();
        summaryItem.put("source", "baidu_fallback");
        summaryItem.put("summary", summary);
        summaryItem.put("results", web);
        return List.of(summaryItem);
    }

    private List<String> splitText(String content, int maxLen) {
        if (content == null) return List.of();
        List<String> result = new ArrayList<>();
        int n = content.length();
        for (int i = 0; i < n; i += maxLen) {
            result.add(content.substring(i, Math.min(n, i + maxLen)));
        }
        return result;
    }
}
