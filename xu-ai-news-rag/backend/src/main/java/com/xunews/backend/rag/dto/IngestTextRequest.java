package com.xunews.backend.rag.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class IngestTextRequest {
    @NotBlank
    private String sourceName;
    private String sourceUrl;

    @NotBlank
    private String title;
    private String url;

    @NotBlank
    private String content;
}
