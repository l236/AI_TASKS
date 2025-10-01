package com.xunews.backend.rss;

import com.rometools.rome.feed.synd.SyndEntry;
import com.rometools.rome.feed.synd.SyndFeed;
import com.rometools.rome.io.SyndFeedInput;
import com.rometools.rome.io.XmlReader;
import com.xunews.backend.rag.RagService;
import com.xunews.backend.rag.dto.IngestTextRequest;
import com.xunews.backend.news.ArticleRepository;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.net.URL;
import java.time.Duration;
import java.util.List;

@Slf4j
@Component
@RequiredArgsConstructor
public class RssScheduler {

    private final RssConfigProperties props;
    private final RagService ragService;
    private final ArticleRepository articleRepository;

    // Run hourly by default; dynamic interval from props is applied via fixedDelayString
    @Scheduled(fixedDelayString = "${rss.interval-minutes:60}000", initialDelay = 20_000)
    public void fetchFeeds() {
        List<String> feeds = props.getFeeds();
        if (feeds == null || feeds.isEmpty()) return;
        log.info("[RSS] Start fetching {} feeds", feeds.size());
        for (String feedUrl : feeds) {
            try {
                SyndFeedInput input = new SyndFeedInput();
                SyndFeed feed = input.build(new XmlReader(new URL(feedUrl)));
                for (SyndEntry entry : feed.getEntries()) {
                    String url = entry.getLink();
                    if (url == null || url.isBlank()) continue;
                    if (articleRepository.existsByUrl(url)) continue; // de-dup

                    // polite delay between requests
                    Thread.sleep(300);

                    String title = entry.getTitle() != null ? entry.getTitle() : "Untitled";
                    String content = extractMainText(url);

                    IngestTextRequest req = new IngestTextRequest();
                    req.setSourceName(feed.getTitle());
                    req.setSourceUrl(feedUrl);
                    req.setTitle(title);
                    req.setUrl(url);
                    req.setContent(content);
                    ragService.ingestText(req);
                    log.info("[RSS] Ingested: {}", title);
                }
            } catch (Exception e) {
                log.warn("[RSS] Failed feed {}: {}", feedUrl, e.getMessage());
            }
        }
        log.info("[RSS] Done");
    }

    private String extractMainText(String url) {
        try {
            Document doc = Jsoup.connect(url)
                    .userAgent("Mozilla/5.0 (compatible; xu-ai-news-rag/0.1; +https://example.com)")
                    .timeout((int) Duration.ofSeconds(15).toMillis())
                    .get();
            // Very simple extraction: use body text; can be replaced with boilerpipe/readability
            return doc.body() != null ? doc.body().text() : "";
        } catch (Exception e) {
            return "";
        }
    }
}
