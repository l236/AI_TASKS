package com.example.todoapp.service;

import com.example.todoapp.dto.TodoRequest;
import com.example.todoapp.model.Todo;
import com.example.todoapp.repository.TodoRepository;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ActiveProfiles;

@SpringBootTest
@ActiveProfiles("test")
class TodoServiceTest {

    @Autowired
    private TodoService todoService;

    @Autowired
    private TodoRepository todoRepository;

    @Test
    void createAndToggleTodo() {
        TodoRequest req = new TodoRequest();
        req.setTitle("测试创建");
        req.setDescription("desc");
        req.setPriority(1);

        Todo created = todoService.create(req);
        Assertions.assertNotNull(created.getId());
        Assertions.assertFalse(created.isCompleted());

        Todo toggled = todoService.toggle(created.getId());
        Assertions.assertTrue(toggled.isCompleted());

        long total = todoRepository.count();
        Assertions.assertEquals(1, total);
    }
}
