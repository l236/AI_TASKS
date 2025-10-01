package com.xunews.backend.analytics;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/analytics")
@Tag(name = "Analytics", description = "Analytics and keyword distribution")
@RequiredArgsConstructor
public class AnalyticsController {

    private final AnalyticsService analyticsService;

    @GetMapping("/keywords/top10")
    @Operation(summary = "Top 10 keywords", description = "Compute keyword frequency from recent chunks")
    public ResponseEntity<List<Map<String, Object>>> top10() {
        return ResponseEntity.ok(analyticsService.topKeywords(10));
    }
}
