package com.xunews.backend.search;

import lombok.RequiredArgsConstructor;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

@Component
@RequiredArgsConstructor
public class BaiduClient {

    @Value("${app.search.baidu.enabled:false}")
    private boolean enabled;

    public List<Map<String, String>> topResults(String query, int topK) {
        if (!enabled) return List.of();
        try {
            String q = URLEncoder.encode(query, StandardCharsets.UTF_8);
            String url = "https://www.baidu.com/s?wd=" + q;
            Document doc = Jsoup.connect(url)
                    .userAgent("Mozilla/5.0 (compatible; xu-ai-news-rag/0.1; +https://example.com)")
                    .timeout((int) Duration.ofSeconds(15).toMillis())
                    .get();
            Elements results = doc.select("div.result, div.result-op");
            List<Map<String, String>> out = new ArrayList<>();
            for (Element el : results) {
                Element a = el.selectFirst("h3 a");
                if (a == null) continue;
                String title = a.text();
                String href = a.absUrl("href");
                if (href == null || href.isBlank()) continue;
                out.add(Map.of(
                        "title", title,
                        "url", href
                ));
                if (out.size() >= topK) break;
            }
            return out;
        } catch (Exception e) {
            return List.of();
        }
    }
}
