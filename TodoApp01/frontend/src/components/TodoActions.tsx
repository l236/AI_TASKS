import React from 'react';
import './TodoActions.css';

interface TodoActionsProps {
  onDeleteCompleted: () => void;
  onDeleteAll: () => void;
  hasCompleted: boolean;
}

const TodoActions: React.FC<TodoActionsProps> = ({
  onDeleteCompleted,
  onDeleteAll,
  hasCompleted,
}) => {
  return (
    <div className="todo-actions">
      <div className="action-buttons">
        <button
          onClick={onDeleteCompleted}
          className="action-btn clear-completed"
          disabled={!hasCompleted}
        >
          清除已完成
        </button>
        <button
          onClick={onDeleteAll}
          className="action-btn clear-all danger"
        >
          清除全部
        </button>
      </div>
    </div>
  );
};

export default TodoActions;
