package com.xunews.backend.analytics;

import com.xunews.backend.news.Chunk;
import com.xunews.backend.news.ChunkRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.*;
import java.util.regex.Pattern;

@Service
@RequiredArgsConstructor
public class AnalyticsService {

    private final ChunkRepository chunkRepository;

    private static final Set<String> STOP_WORDS = Set.of(
            "the","a","an","and","or","but","of","to","in","on","for","with","is","are","was","were",
            "这","那","的","了","和","或","与","是","在","对","及","并","等","中","为","于"
    );
    private static final Pattern SPLIT = Pattern.compile("[^\u4e00-\u9fa5A-Za-z0-9]+");

    public List<Map<String, Object>> topKeywords(int topN) {
        List<Chunk> chunks = chunkRepository.findTop1000ByOrderByIdDesc();
        Map<String, Integer> freq = new HashMap<>();
        for (Chunk c : chunks) {
            if (c.getText() == null) continue;
            String[] tokens = SPLIT.split(c.getText().toLowerCase());
            for (String t : tokens) {
                if (t.isBlank()) continue;
                if (STOP_WORDS.contains(t)) continue;
                freq.merge(t, 1, Integer::sum);
            }
        }
        return freq.entrySet().stream()
                .sorted((a,b) -> Integer.compare(b.getValue(), a.getValue()))
                .limit(topN)
                .map(e -> Map.of("keyword", e.getKey(), "count", e.getValue()))
                .toList();
    }
}
