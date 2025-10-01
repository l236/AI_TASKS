package com.xunews.backend.rag;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.xunews.backend.AbstractIntegrationTest;
import com.xunews.backend.auth.AuthService;
import com.xunews.backend.auth.dto.RegisterRequest;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MvcResult;

import java.util.List;
import java.util.Map;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyList;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

public class RagControllerTest extends AbstractIntegrationTest {

    @Autowired
    private ObjectMapper objectMapper;

    @Autowired
    private AuthService authService;

    @MockBean
    private RagClient ragClient;

    private String token;

    @BeforeEach
    void setupUser() {
        RegisterRequest rr = new RegisterRequest();
        rr.setUsername("ragtester");
        rr.setPassword("pass123");
        rr.setEmail("r@t.com");
        token = authService.register(rr);
    }

    @Test
    void ingest_text_and_search_should_work() throws Exception {
        when(ragClient.upsert(anyList())).thenReturn(2);
        when(ragClient.search(anyString(), anyInt())).thenReturn(List.of(
                Map.of("id", "1", "text", "hello world", "score", 0.9)
        ));

        String body = "{" +
                "\"sourceName\":\"Demo\"," +
                "\"title\":\"Title\"," +
                "\"content\":\"hello world content\"}";
        mockMvc.perform(post("/api/rag/ingest/text")
                        .header("Authorization", authHeader(token))
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(body))
                .andExpect(status().isOk());

        String q = "{\"query\":\"hello\",\"topK\":3}";
        MvcResult res = mockMvc.perform(post("/api/rag/search")
                        .header("Authorization", authHeader(token))
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(q))
                .andExpect(status().isOk())
                .andReturn();
        JsonNode json = objectMapper.readTree(res.getResponse().getContentAsString());
        assertThat(json.isArray()).isTrue();
    }
}
