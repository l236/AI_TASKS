package com.xunews.backend.rag.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class SearchRequest {
    @NotBlank
    private String query;
    private int topK = 5;
}
