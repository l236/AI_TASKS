package com.xunews.backend.rag;

import com.xunews.backend.rag.dto.IngestTextRequest;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.DataFormatter;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.io.InputStream;
import java.util.*;

@RestController
@RequestMapping("/api/rag/ingest")
@Tag(name = "Ingest", description = "Excel and file ingestion APIs")
@RequiredArgsConstructor
public class IngestExcelController {

    private final RagService ragService;

    @PostMapping(value = "/excel", consumes = MediaType.MULTIPART_FORM_DATA_VALUE)
    @Operation(summary = "Ingest from Excel", description = "Excel columns: sourceName, sourceUrl, title, url, content")
    public ResponseEntity<Map<String, Object>> ingestExcel(@RequestPart("file") MultipartFile file) throws Exception {
        int success = 0;
        int failed = 0;
        List<String> errors = new ArrayList<>();

        try (InputStream is = file.getInputStream(); XSSFWorkbook wb = new XSSFWorkbook(is)) {
            Sheet sheet = wb.getSheetAt(0);
            boolean headerSkipped = false;
            for (Row row : sheet) {
                // skip header row if present
                if (!headerSkipped) { headerSkipped = true; continue; }
                try {
                    String sourceName = getCellString(row, 0);
                    String sourceUrl = getCellString(row, 1);
                    String title = getCellString(row, 2);
                    String url = getCellString(row, 3);
                    String content = getCellString(row, 4);
                    IngestTextRequest req = new IngestTextRequest();
                    req.setSourceName(sourceName);
                    req.setSourceUrl(sourceUrl);
                    req.setTitle(title);
                    req.setUrl(url);
                    req.setContent(content);
                    ragService.ingestText(req);
                    success++;
                } catch (Exception ex) {
                    failed++;
                    errors.add("Row " + row.getRowNum() + ": " + ex.getMessage());
                }
            }
        }

        return ResponseEntity.ok(Map.of(
                "success", success,
                "failed", failed,
                "errors", errors
        ));
    }

    private String getCellString(Row row, int idx) {
        if (row.getCell(idx) == null) return "";
        DataFormatter formatter = new DataFormatter();
        return formatter.formatCellValue(row.getCell(idx));
    }
}
