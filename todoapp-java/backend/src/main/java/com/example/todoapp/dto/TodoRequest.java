package com.example.todoapp.dto;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.time.LocalDateTime;

@Data
public class TodoRequest {
    @NotBlank
    private String title;
    private String description;
    private Integer priority;
    private LocalDateTime dueDate;
    private Boolean completed; // used in update
}
