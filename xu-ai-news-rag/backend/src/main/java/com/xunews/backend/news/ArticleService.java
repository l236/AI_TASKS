package com.xunews.backend.news;

import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
@RequiredArgsConstructor
public class ArticleService {

    private final SourceRepository sourceRepository;
    private final ArticleRepository articleRepository;

    @Transactional
    public Article createOrGetArticle(String sourceName, String sourceUrl, String title, String url, String content) {
        Source source = sourceRepository.findByName(sourceName)
                .orElseGet(() -> sourceRepository.save(Source.builder().name(sourceName).url(sourceUrl).build()));
        Article article = Article.builder()
                .source(source)
                .title(title)
                .url(url)
                .content(content)
                .build();
        return articleRepository.save(article);
    }
}
