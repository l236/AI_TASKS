import React from 'react';
import { Todo } from '../types/todo';
import TodoItem from './TodoItem';
import './TodoList.css';

interface TodoListProps {
  todos: Todo[];
  loading: boolean;
  onToggleComplete: (id: number, completed: boolean) => void;
  onDeleteTodo: (id: number) => void;
}

const TodoList: React.FC<TodoListProps> = ({
  todos,
  loading,
  onToggleComplete,
  onDeleteTodo,
}) => {
  if (loading) {
    return (
      <div className="todo-list loading">
        <div className="loading-spinner">加载中...</div>
      </div>
    );
  }

  if (todos.length === 0) {
    return (
      <div className="todo-list empty">
        <div className="empty-state">
          <h3>📝</h3>
          <p>还没有待办事项，添加一个吧！</p>
        </div>
      </div>
    );
  }

  return (
    <div className="todo-list">
      <ol className="todo-items">
        {todos.map((todo) => (
          <TodoItem
            key={todo.id}
            todo={todo}
            onToggleComplete={onToggleComplete}
            onDeleteTodo={onDeleteTodo}
          />
        ))}
      </ol>
    </div>
  );
};

export default TodoList;
