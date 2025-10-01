package com.xunews.backend.analytics;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.xunews.backend.AbstractIntegrationTest;
import com.xunews.backend.auth.AuthService;
import com.xunews.backend.auth.dto.RegisterRequest;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MvcResult;

import static org.assertj.core.api.Assertions.assertThat;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

public class AnalyticsControllerTest extends AbstractIntegrationTest {

    @Autowired
    private ObjectMapper objectMapper;

    @Autowired
    private AuthService authService;

    private String token;

    @BeforeEach
    void setupUser() {
        RegisterRequest rr = new RegisterRequest();
        rr.setUsername("analyst");
        rr.setPassword("pass123");
        rr.setEmail("a@a.com");
        token = authService.register(rr);
    }

    @Test
    void top10_keywords_should_return_array() throws Exception {
        MvcResult res = mockMvc.perform(get("/api/analytics/keywords/top10")
                        .header("Authorization", authHeader(token))
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andReturn();
        JsonNode json = objectMapper.readTree(res.getResponse().getContentAsString());
        assertThat(json.isArray()).isTrue();
    }
}
