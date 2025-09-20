import React from 'react';
import { FilterStatus } from '../types/todo';
import './TodoFilters.css';

interface TodoFiltersProps {
  currentFilter: FilterStatus;
  onFilterChange: (filter: FilterStatus) => void;
}

const TodoFilters: React.FC<TodoFiltersProps> = ({
  currentFilter,
  onFilterChange,
}) => {
  const filters: { key: FilterStatus; label: string }[] = [
    { key: 'all', label: '全部' },
    { key: 'active', label: '未完成' },
    { key: 'completed', label: '已完成' },
  ];

  return (
    <div className="todo-filters">
      <div className="filter-buttons">
        {filters.map((filter) => (
          <button
            key={filter.key}
            onClick={() => onFilterChange(filter.key)}
            className={`filter-btn ${currentFilter === filter.key ? 'active' : ''}`}
          >
            {filter.label}
          </button>
        ))}
      </div>
    </div>
  );
};

export default TodoFilters;
