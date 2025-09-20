import React, { useState } from 'react';
import { Todo } from '../types/todo';
import './TodoItem.css';

interface TodoItemProps {
  todo: Todo;
  onToggleComplete: (id: number, completed: boolean) => void;
  onDeleteTodo: (id: number) => void;
}

const TodoItem: React.FC<TodoItemProps> = ({
  todo,
  onToggleComplete,
  onDeleteTodo,
}) => {
  const [isEditing, setIsEditing] = useState(false);
  const [editTitle, setEditTitle] = useState(todo.title);
  const [editDescription, setEditDescription] = useState(todo.description || '');

  const handleToggleComplete = () => {
    onToggleComplete(todo.id, !todo.completed);
  };

  const handleDelete = () => {
    if (window.confirm('确定要删除这个待办事项吗？')) {
      onDeleteTodo(todo.id);
    }
  };

  const handleEdit = () => {
    setIsEditing(true);
  };

  const handleSave = () => {
    if (editTitle.trim()) {
      // 这里可以调用更新API，暂时简化处理
      setIsEditing(false);
    }
  };

  const handleCancel = () => {
    setEditTitle(todo.title);
    setEditDescription(todo.description || '');
    setIsEditing(false);
  };

  const formatDate = (dateString: string) => {
    return new Date(dateString).toLocaleString('zh-CN');
  };

  return (
    <li className={`todo-item ${todo.completed ? 'completed' : ''}`}>
      <div className="todo-content">
        {isEditing ? (
          <div className="edit-form">
            <input
              type="text"
              value={editTitle}
              onChange={(e) => setEditTitle(e.target.value)}
              className="edit-input"
              autoFocus
            />
            <textarea
              value={editDescription}
              onChange={(e) => setEditDescription(e.target.value)}
              className="edit-textarea"
              rows={2}
            />
            <div className="edit-actions">
              <button onClick={handleSave} className="save-btn">保存</button>
              <button onClick={handleCancel} className="cancel-btn">取消</button>
            </div>
          </div>
        ) : (
          <>
            <div className="todo-text">
              <h4 className="todo-title">{todo.title}</h4>
              {todo.description && (
                <p className="todo-description">{todo.description}</p>
              )}
              <div className="todo-meta">
                <span className="todo-date">
                  创建于: {formatDate(todo.created_at)}
                </span>
                {todo.updated_at !== todo.created_at && (
                  <span className="todo-date">
                    更新于: {formatDate(todo.updated_at)}
                  </span>
                )}
              </div>
            </div>
            <div className="todo-actions">
              <button
                onClick={handleToggleComplete}
                className={`complete-btn ${todo.completed ? 'completed' : ''}`}
              >
                {todo.completed ? '已完成' : '完成'}
              </button>
              <button onClick={handleEdit} className="edit-btn">
                编辑
              </button>
              <button onClick={handleDelete} className="delete-btn">
                删除
              </button>
            </div>
          </>
        )}
      </div>
    </li>
  );
};

export default TodoItem;
