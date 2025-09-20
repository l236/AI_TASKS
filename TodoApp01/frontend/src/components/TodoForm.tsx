import React, { useState } from 'react';
import './TodoForm.css';

interface TodoFormProps {
  onCreateTodo: (title: string, description?: string) => void;
}

const TodoForm: React.FC<TodoFormProps> = ({ onCreateTodo }) => {
  const [title, setTitle] = useState('');
  const [description, setDescription] = useState('');

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    if (title.trim()) {
      onCreateTodo(title.trim(), description.trim() || undefined);
      setTitle('');
      setDescription('');
    }
  };

  return (
    <form className="todo-form" onSubmit={handleSubmit}>
      <div className="form-group">
        <input
          type="text"
          value={title}
          onChange={(e) => setTitle(e.target.value)}
          placeholder="请输入待办事项标题..."
          className="todo-input"
          required
        />
        <button type="submit" className="add-button">
          添加
        </button>
      </div>
      <div className="form-group">
        <textarea
          value={description}
          onChange={(e) => setDescription(e.target.value)}
          placeholder="请输入详细描述（可选）..."
          className="todo-description"
          rows={2}
        />
      </div>
    </form>
  );
};

export default TodoForm;
