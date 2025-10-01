package com.xunews.backend.rag;

import com.xunews.backend.rag.dto.IngestTextRequest;
import com.xunews.backend.rag.dto.SearchRequest;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/rag")
@Tag(name = "RAG", description = "Knowledge base ingest and search APIs")
@RequiredArgsConstructor
public class RagController {

    private final RagService ragService;

    @PostMapping("/ingest/text")
    @Operation(summary = "Ingest plain text article", description = "Save metadata and chunks; upsert vectors to store")
    public ResponseEntity<Map<String, Object>> ingestText(@Valid @RequestBody IngestTextRequest req) {
        return ResponseEntity.ok(ragService.ingestText(req));
    }

    @PostMapping("/search")
    @Operation(summary = "Search knowledge base", description = "Vector search with topK, returns matched chunks and metadata")
    public ResponseEntity<List<Map<String, Object>>> search(@Valid @RequestBody SearchRequest req) {
        return ResponseEntity.ok(ragService.search(req.getQuery(), req.getTopK()));
    }
}
