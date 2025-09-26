package com.example.todoapp.service;

import com.example.todoapp.dto.TodoRequest;
import com.example.todoapp.exception.NotFoundException;
import com.example.todoapp.model.Todo;
import com.example.todoapp.repository.TodoRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class TodoService {

    private final TodoRepository todoRepository;

    public Page<Todo> list(Boolean completed, int limit, int offset) {
        int page = offset / Math.max(1, limit);
        Pageable pageable = PageRequest.of(page, limit);
        if (completed == null) {
            return todoRepository.findAll(pageable);
        }
        return todoRepository.findByCompleted(completed, pageable);
    }

    public Todo create(TodoRequest req) {
        Todo todo = Todo.builder()
                .title(req.getTitle())
                .description(req.getDescription())
                .priority(req.getPriority() == null ? 0 : req.getPriority())
                .dueDate(req.getDueDate())
                .completed(false)
                .build();
        return todoRepository.save(todo);
    }

    public Todo update(Long id, TodoRequest req) {
        Todo todo = getById(id);
        if (req.getTitle() != null) todo.setTitle(req.getTitle());
        if (req.getDescription() != null) todo.setDescription(req.getDescription());
        if (req.getPriority() != null) todo.setPriority(req.getPriority());
        if (req.getDueDate() != null) todo.setDueDate(req.getDueDate());
        if (req.getCompleted() != null) todo.setCompleted(req.getCompleted());
        return todoRepository.save(todo);
    }

    public void delete(Long id) {
        if (!todoRepository.existsById(id)) {
            throw new NotFoundException("Todo not found: id=" + id);
        }
        todoRepository.deleteById(id);
    }

    public Todo toggle(Long id) {
        Todo todo = getById(id);
        todo.setCompleted(!todo.isCompleted());
        return todoRepository.save(todo);
    }

    public long deleteCompleted() {
        long count = todoRepository.findAll().stream().filter(Todo::isCompleted).count();
        todoRepository.deleteAll(
                todoRepository.findAll().stream().filter(Todo::isCompleted).toList()
        );
        return count;
    }

    public long deleteAllTodos() {
        long count = todoRepository.count();
        todoRepository.deleteAll();
        return count;
    }

    private Todo getById(Long id) {
        return todoRepository.findById(id)
                .orElseThrow(() -> new NotFoundException("Todo not found"));
    }
}
