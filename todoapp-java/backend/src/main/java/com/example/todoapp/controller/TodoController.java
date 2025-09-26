package com.example.todoapp.controller;

import com.example.todoapp.dto.ApiResponse;
import com.example.todoapp.dto.TodoRequest;
import com.example.todoapp.model.Todo;
import com.example.todoapp.service.TodoService;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.data.domain.Page;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/v1/todos")
@RequiredArgsConstructor
public class TodoController {

    private final TodoService todoService;

    @GetMapping
    public ResponseEntity<ApiResponse<?>> list(
            @RequestParam(value = "completed", required = false) Boolean completed,
            @RequestParam(value = "limit", defaultValue = "100") int limit,
            @RequestParam(value = "offset", defaultValue = "0") int offset
    ) {
        Page<Todo> page = todoService.list(completed, limit, offset);
        return ResponseEntity.ok(
                ApiResponse.builder()
                        .code(200)
                        .message("success")
                        .data(page.getContent())
                        .total(page.getTotalElements())
                        .build()
        );
    }

    @PostMapping
    public ResponseEntity<ApiResponse<?>> create(@Valid @RequestBody TodoRequest req) {
        Todo created = todoService.create(req);
        return ResponseEntity.status(HttpStatus.CREATED).body(
                ApiResponse.builder()
                        .code(201)
                        .message("Todo created successfully")
                        .data(created)
                        .build()
        );
    }

    @PutMapping("/{id}")
    public ResponseEntity<ApiResponse<?>> update(@PathVariable("id") Long id, @RequestBody TodoRequest req) {
        Todo updated = todoService.update(id, req);
        return ResponseEntity.ok(
                ApiResponse.builder()
                        .code(200)
                        .message("Todo updated successfully")
                        .data(updated)
                        .build()
        );
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<ApiResponse<?>> delete(@PathVariable("id") Long id) {
        todoService.delete(id);
        return ResponseEntity.ok(
                ApiResponse.builder()
                        .code(200)
                        .message("Todo deleted successfully")
                        .build()
        );
    }

    @PatchMapping("/{id}/toggle")
    public ResponseEntity<ApiResponse<?>> toggle(@PathVariable("id") Long id) {
        Todo toggled = todoService.toggle(id);
        return ResponseEntity.ok(
                ApiResponse.builder()
                        .code(200)
                        .message("Todo status toggled successfully")
                        .data(new SimpleToggle(toggled.getId(), toggled.isCompleted(), toggled.getUpdatedAt().toString()))
                        .build()
        );
    }

    @DeleteMapping("/completed")
    public ResponseEntity<ApiResponse<?>> deleteCompleted() {
        long count = todoService.deleteCompleted();
        return ResponseEntity.ok(
                ApiResponse.builder()
                        .code(200)
                        .message("Completed todos deleted successfully")
                        .total(count)
                        .build()
        );
    }

    @DeleteMapping("/all")
    public ResponseEntity<ApiResponse<?>> deleteAll() {
        long count = todoService.deleteAllTodos();
        return ResponseEntity.ok(
                ApiResponse.builder()
                        .code(200)
                        .message("All todos deleted successfully")
                        .total(count)
                        .build()
        );
    }

    private record SimpleToggle(Long id, boolean completed, String updated_at) {}
}
