import React, { useState, useEffect, useCallback } from 'react';
import { Todo, FilterStatus } from '../types/todo';
import { todoApi } from '../services/api';
import TodoForm from './TodoForm';
import TodoList from './TodoList';
import TodoFilters from './TodoFilters';
import TodoActions from './TodoActions';
import './TodoApp.css';

const TodoApp: React.FC = () => {
  const [todos, setTodos] = useState<Todo[]>([]);
  const [filter, setFilter] = useState<FilterStatus>('all');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  // 加载待办事项
  const loadTodos = useCallback(async () => {
    try {
      setLoading(true);
      setError(null);
      const data = await todoApi.getTodos(filter);
      setTodos(data);
    } catch (err) {
      setError('加载待办事项失败，请检查网络连接');
      console.error('Error loading todos:', err);
    } finally {
      setLoading(false);
    }
  }, [filter]);

  // 创建待办事项
  const handleCreateTodo = async (title: string, description?: string) => {
    try {
      setError(null);
      const newTodo = await todoApi.createTodo({ title, description });
      setTodos(prev => [newTodo, ...prev]);
    } catch (err) {
      setError('创建待办事项失败');
      console.error('Error creating todo:', err);
    }
  };

  // 更新待办事项
  const handleUpdateTodo = async (id: number, updates: Partial<Todo>) => {
    try {
      setError(null);
      const updatedTodo = await todoApi.updateTodo(id, updates);
      setTodos(prev => prev.map(todo => todo.id === id ? updatedTodo : todo));
    } catch (err) {
      setError('更新待办事项失败');
      console.error('Error updating todo:', err);
    }
  };

  // 删除待办事项
  const handleDeleteTodo = async (id: number) => {
    try {
      setError(null);
      await todoApi.deleteTodo(id);
      setTodos(prev => prev.filter(todo => todo.id !== id));
    } catch (err) {
      setError('删除待办事项失败');
      console.error('Error deleting todo:', err);
    }
  };

  // 切换完成状态
  const handleToggleComplete = (id: number, completed: boolean) => {
    handleUpdateTodo(id, { completed });
  };

  // 删除已完成的待办事项
  const handleDeleteCompleted = async () => {
    if (!window.confirm('确定要删除所有已完成的待办事项吗？')) {
      return;
    }

    try {
      setError(null);
      await todoApi.deleteCompletedTodos();
      setTodos(prev => prev.filter(todo => !todo.completed));
    } catch (err) {
      setError('删除已完成的待办事项失败');
      console.error('Error deleting completed todos:', err);
    }
  };

  // 删除所有待办事项
  const handleDeleteAll = async () => {
    if (!window.confirm('确定要删除所有待办事项吗？')) {
      return;
    }

    try {
      setError(null);
      await todoApi.deleteAllTodos();
      setTodos([]);
    } catch (err) {
      setError('删除所有待办事项失败');
      console.error('Error deleting all todos:', err);
    }
  };

  // 过滤待办事项
  const filteredTodos = todos.filter(todo => {
    switch (filter) {
      case 'active':
        return !todo.completed;
      case 'completed':
        return todo.completed;
      default:
        return true;
    }
  });

  // 获取统计信息
  const stats = {
    total: todos.length,
    active: todos.filter(todo => !todo.completed).length,
    completed: todos.filter(todo => todo.completed).length,
  };

  // 当过滤器改变时重新加载数据
  useEffect(() => {
    loadTodos();
  }, [filter, loadTodos]);

  return (
    <div className="todo-app">
      <header className="todo-header">
        <h1>我的待办事项</h1>
        <div className="todo-stats">
          <span>总计: {stats.total}</span>
          <span>未完成: {stats.active}</span>
          <span>已完成: {stats.completed}</span>
        </div>
      </header>

      <main className="todo-main">
        <TodoForm onCreateTodo={handleCreateTodo} />
        
        <TodoFilters 
          currentFilter={filter} 
          onFilterChange={setFilter} 
        />
        
        {error && (
          <div className="error-message">
            {error}
          </div>
        )}
        
        <TodoList
          todos={filteredTodos}
          loading={loading}
          onToggleComplete={handleToggleComplete}
          onDeleteTodo={handleDeleteTodo}
        />
        
        {todos.length > 0 && (
          <TodoActions
            onDeleteCompleted={handleDeleteCompleted}
            onDeleteAll={handleDeleteAll}
            hasCompleted={stats.completed > 0}
          />
        )}
      </main>
    </div>
  );
};

export default TodoApp;
