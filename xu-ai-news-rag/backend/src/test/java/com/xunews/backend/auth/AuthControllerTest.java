package com.xunews.backend.auth;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.xunews.backend.AbstractIntegrationTest;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MvcResult;

import static org.assertj.core.api.Assertions.assertThat;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

public class AuthControllerTest extends AbstractIntegrationTest {

    @Autowired
    private ObjectMapper objectMapper;

    @Test
    void register_and_login_should_return_tokens() throws Exception {
        // register
        String regBody = "{" +
                "\"username\":\"user1\"," +
                "\"password\":\"pass123\"," +
                "\"email\":\"u1@example.com\"}";
        MvcResult reg = mockMvc.perform(post("/api/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(regBody))
                .andExpect(status().isOk())
                .andReturn();
        JsonNode regJson = objectMapper.readTree(reg.getResponse().getContentAsString());
        assertThat(regJson.has("token")).isTrue();

        // login
        String loginBody = "{" +
                "\"username\":\"user1\"," +
                "\"password\":\"pass123\"}";
        MvcResult log = mockMvc.perform(post("/api/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(loginBody))
                .andExpect(status().isOk())
                .andReturn();
        JsonNode logJson = objectMapper.readTree(log.getResponse().getContentAsString());
        assertThat(logJson.has("token")).isTrue();
    }
}
