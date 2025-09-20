# 待办事项应用技术架构文档

## 项目概述

本项目是一个全栈待办事项应用，采用前后端分离架构，提供完整的CRUD功能。

## 技术栈

### 前端
- **框架**: React 18+
- **语言**: TypeScript
- **样式**: CSS3 + CSS Modules
- **HTTP客户端**: Axios
- **状态管理**: React Hooks (useState, useEffect)
- **构建工具**: Vite

### 后端
- **框架**: FastAPI
- **语言**: Python 3.8+
- **数据库**: SQLite
- **ORM**: SQLAlchemy
- **数据验证**: Pydantic
- **CORS**: FastAPI CORS middleware

### 数据库
- **类型**: SQLite
- **表**: todos (待办事项表)

## 数据库设计

### 表结构

```sql
-- 创建待办事项表
CREATE TABLE todos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title VARCHAR(255) NOT NULL,
    description TEXT,
    completed BOOLEAN DEFAULT FALSE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 创建索引
CREATE INDEX idx_todos_completed ON todos(completed);
CREATE INDEX idx_todos_created_at ON todos(created_at);
```

### 表字段说明

| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| id | INTEGER | PRIMARY KEY | 主键，自增 |
| title | VARCHAR(255) | NOT NULL | 待办事项标题 |
| description | TEXT | NULL | 待办事项描述 |
| completed | BOOLEAN | DEFAULT FALSE | 是否完成 |
| created_at | DATETIME | DEFAULT CURRENT_TIMESTAMP | 创建时间 |
| updated_at | DATETIME | DEFAULT CURRENT_TIMESTAMP | 更新时间 |

## API接口设计

### 基础信息
- **Base URL**: `http://localhost:8000/api/v1`
- **Content-Type**: `application/json`
- **字符编码**: UTF-8

### 接口列表

#### 1. 获取所有待办事项
- **URL**: `GET /todos`
- **描述**: 获取所有待办事项列表
- **查询参数**:
  - `status` (可选): `all` | `active` | `completed` - 过滤状态
- **响应示例**:
```json
{
  "code": 200,
  "message": "success",
  "data": [
    {
      "id": 1,
      "title": "学习React",
      "description": "完成React基础教程",
      "completed": false,
      "created_at": "2024-01-01T10:00:00",
      "updated_at": "2024-01-01T10:00:00"
    }
  ]
}
```

#### 2. 创建待办事项
- **URL**: `POST /todos`
- **描述**: 创建新的待办事项
- **请求体**:
```json
{
  "title": "学习React",
  "description": "完成React基础教程"
}
```
- **响应示例**:
```json
{
  "code": 201,
  "message": "Todo created successfully",
  "data": {
    "id": 1,
    "title": "学习React",
    "description": "完成React基础教程",
    "completed": false,
    "created_at": "2024-01-01T10:00:00",
    "updated_at": "2024-01-01T10:00:00"
  }
}
```

#### 3. 更新待办事项
- **URL**: `PUT /todos/{todo_id}`
- **描述**: 更新指定待办事项
- **路径参数**:
  - `todo_id`: 待办事项ID
- **请求体**:
```json
{
  "title": "学习React (已更新)",
  "description": "完成React基础教程和进阶内容",
  "completed": true
}
```
- **响应示例**:
```json
{
  "code": 200,
  "message": "Todo updated successfully",
  "data": {
    "id": 1,
    "title": "学习React (已更新)",
    "description": "完成React基础教程和进阶内容",
    "completed": true,
    "created_at": "2024-01-01T10:00:00",
    "updated_at": "2024-01-01T11:00:00"
  }
}
```

#### 4. 删除待办事项
- **URL**: `DELETE /todos/{todo_id}`
- **描述**: 删除指定待办事项
- **路径参数**:
  - `todo_id`: 待办事项ID
- **响应示例**:
```json
{
  "code": 200,
  "message": "Todo deleted successfully",
  "data": null
}
```

#### 5. 批量删除已完成事项
- **URL**: `DELETE /todos/completed`
- **描述**: 删除所有已完成的待办事项
- **响应示例**:
```json
{
  "code": 200,
  "message": "Completed todos deleted successfully",
  "data": {
    "deleted_count": 3
  }
}
```

#### 6. 清空所有待办事项
- **URL**: `DELETE /todos/all`
- **描述**: 删除所有待办事项
- **响应示例**:
```json
{
  "code": 200,
  "message": "All todos deleted successfully",
  "data": {
    "deleted_count": 5
  }
}
```

### 错误响应格式

```json
{
  "code": 400,
  "message": "Validation error",
  "data": {
    "detail": "Title is required"
  }
}
```

### HTTP状态码

| 状态码 | 说明 |
|--------|------|
| 200 | 成功 |
| 201 | 创建成功 |
| 400 | 请求参数错误 |
| 404 | 资源不存在 |
| 500 | 服务器内部错误 |

## 项目结构

```
TodoApp01/
├── backend/                 # 后端目录
│   ├── app/
│   │   ├── __init__.py
│   │   ├── main.py         # FastAPI应用入口
│   │   ├── models.py       # 数据模型
│   │   ├── schemas.py      # Pydantic模型
│   │   ├── database.py     # 数据库配置
│   │   └── routers/        # 路由模块
│   │       └── todos.py    # 待办事项路由
│   ├── requirements.txt    # Python依赖
│   ├── database.db        # SQLite数据库文件
│   └── README.md          # 后端说明文档
├── frontend/               # 前端目录
│   ├── src/
│   │   ├── components/     # React组件
│   │   ├── services/       # API服务
│   │   ├── types/          # TypeScript类型
│   │   ├── styles/         # 样式文件
│   │   └── App.tsx         # 主应用组件
│   ├── package.json        # 前端依赖
│   └── README.md          # 前端说明文档
├── req.md                 # 需求文档
└── TECH_ARCHITECTURE.md   # 技术架构文档
```

## 开发环境要求

### 后端环境
- Python 3.8+
- pip 或 conda
- SQLite 3

### 前端环境
- Node.js 16+
- npm 或 yarn

## 部署说明

### 开发环境启动
1. 启动后端: `cd backend && uvicorn app.main:app --reload`
2. 启动前端: `cd frontend && npm run dev`

### 生产环境
- 后端: 使用Gunicorn + Uvicorn
- 前端: 构建静态文件，使用Nginx托管
- 数据库: 可升级到PostgreSQL或MySQL

## 安全考虑

1. **输入验证**: 使用Pydantic进行数据验证
2. **SQL注入防护**: 使用SQLAlchemy ORM
3. **CORS配置**: 限制跨域请求来源
4. **错误处理**: 统一错误响应格式，避免敏感信息泄露

## 性能优化

1. **数据库索引**: 为常用查询字段创建索引
2. **分页查询**: 大量数据时支持分页
3. **缓存策略**: 可考虑添加Redis缓存
4. **前端优化**: 使用React.memo和useMemo优化渲染
