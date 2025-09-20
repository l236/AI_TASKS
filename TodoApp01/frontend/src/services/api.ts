import axios from 'axios';
import { Todo, ApiResponse, TodoCreate, TodoUpdate, FilterStatus } from '../types/todo';

const API_BASE_URL = 'http://localhost:8000/api/v1';

const api = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json',
  },
});

// 请求拦截器
api.interceptors.request.use(
  (config) => {
    console.log('API Request:', config.method?.toUpperCase(), config.url);
    return config;
  },
  (error) => {
    console.error('API Request Error:', error);
    return Promise.reject(error);
  }
);

// 响应拦截器
api.interceptors.response.use(
  (response) => {
    console.log('API Response:', response.status, response.data);
    return response;
  },
  (error) => {
    console.error('API Response Error:', error.response?.status, error.response?.data);
    return Promise.reject(error);
  }
);

export const todoApi = {
  // 获取待办事项列表
  getTodos: async (status: FilterStatus = 'all'): Promise<Todo[]> => {
    const response = await api.get<ApiResponse<{ todos: Todo[] }>>(`/todos/?status=${status}`);
    return response.data.data.todos;
  },

  // 创建待办事项
  createTodo: async (todo: TodoCreate): Promise<Todo> => {
    const response = await api.post<ApiResponse<{ todo: Todo }>>('/todos/', todo);
    return response.data.data.todo;
  },

  // 更新待办事项
  updateTodo: async (id: number, todo: TodoUpdate): Promise<Todo> => {
    const response = await api.put<ApiResponse<{ todo: Todo }>>(`/todos/${id}/`, todo);
    return response.data.data.todo;
  },

  // 删除待办事项
  deleteTodo: async (id: number): Promise<void> => {
    await api.delete(`/todos/${id}/`);
  },

  // 删除已完成的待办事项
  deleteCompletedTodos: async (): Promise<{ deleted_count: number }> => {
    const response = await api.delete<ApiResponse<{ deleted_count: number }>>('/todos/completed/');
    return response.data.data;
  },

  // 删除所有待办事项
  deleteAllTodos: async (): Promise<{ deleted_count: number }> => {
    const response = await api.delete<ApiResponse<{ deleted_count: number }>>('/todos/all/');
    return response.data.data;
  },
};

export default api;
