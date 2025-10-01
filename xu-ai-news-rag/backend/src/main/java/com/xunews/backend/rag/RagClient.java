package com.xunews.backend.rag;

import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Component
@RequiredArgsConstructor
public class RagClient {

    private final RestTemplate restTemplate;

    @Value("${RAG_WORKER_URL:http://localhost:8001}")
    private String ragWorkerUrl;

    public int upsert(List<Map<String, Object>> items) {
        String url = ragWorkerUrl + "/upsert";
        Map<String, Object> body = new HashMap<>();
        body.put("items", items);
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        Map<?, ?> resp = restTemplate.postForObject(url, new HttpEntity<>(body, headers), Map.class);
        if (resp == null) return 0;
        Object inserted = resp.get("inserted");
        return inserted instanceof Number ? ((Number) inserted).intValue() : 0;
    }

    public List<Map<String, Object>> search(String query, int topK) {
        String url = ragWorkerUrl + "/search";
        Map<String, Object> body = Map.of("query", query, "top_k", topK);
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        Map<?, ?> resp = restTemplate.postForObject(url, new HttpEntity<>(body, headers), Map.class);
        if (resp == null) return List.of();
        Object results = resp.get("results");
        if (results instanceof List<?> list) {
            //noinspection unchecked
            return (List<Map<String, Object>>) (List<?>) list;
        }
        return List.of();
    }

    public String summarize(String query, List<String> snippets) {
        String url = ragWorkerUrl + "/summarize";
        Map<String, Object> body = new HashMap<>();
        body.put("query", query);
        body.put("snippets", snippets);
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        Map<?, ?> resp = restTemplate.postForObject(url, new HttpEntity<>(body, headers), Map.class);
        if (resp == null) return "";
        Object summary = resp.get("summary");
        return summary != null ? summary.toString() : "";
    }
}
