# Todo App Backend

基于 FastAPI 和 SQLite 的待办事项应用后端服务。

## 技术栈

- **FastAPI**: 现代、快速的 Web 框架
- **SQLAlchemy**: Python SQL 工具包和对象关系映射
- **SQLite**: 轻量级数据库
- **Pydantic**: 数据验证和序列化
- **Uvicorn**: ASGI 服务器

## 项目结构

```
backend/
├── app/
│   ├── __init__.py
│   ├── main.py              # FastAPI 应用入口
│   ├── database.py          # 数据库配置和模型
│   ├── schemas.py           # Pydantic 数据模型
│   └── routers/
│       ├── __init__.py
│       └── todos.py         # 待办事项路由
├── requirements.txt         # Python 依赖
├── database.db             # SQLite 数据库文件（自动生成）
└── README.md               # 说明文档
```

## 安装和运行

### 1. 安装依赖

```bash
cd backend
pip install -r requirements.txt
```

### 2. 运行开发服务器

```bash
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```

或者使用 Python 直接运行：

```bash
python -m app.main
```

### 3. 访问 API 文档

- Swagger UI: http://localhost:8000/docs
- ReDoc: http://localhost:8000/redoc

## API 接口

### 基础信息
- **Base URL**: `http://localhost:8000/api/v1`
- **Content-Type**: `application/json`

### 接口列表

#### 1. 获取待办事项列表
```http
GET /api/v1/todos?status=all
```

**查询参数**:
- `status` (可选): `all` | `active` | `completed`

**响应示例**:
```json
{
  "code": 200,
  "message": "success",
  "data": {
    "todos": [
      {
        "id": 1,
        "title": "学习 React",
        "description": "完成 React 基础教程",
        "completed": false,
        "created_at": "2024-01-01T10:00:00",
        "updated_at": "2024-01-01T10:00:00"
      }
    ]
  }
}
```

#### 2. 创建待办事项
```http
POST /api/v1/todos
```

**请求体**:
```json
{
  "title": "学习 React",
  "description": "完成 React 基础教程"
}
```

#### 3. 更新待办事项
```http
PUT /api/v1/todos/{todo_id}
```

**请求体**:
```json
{
  "title": "学习 React (已更新)",
  "description": "完成 React 基础教程和进阶内容",
  "completed": true
}
```

#### 4. 删除待办事项
```http
DELETE /api/v1/todos/{todo_id}
```

#### 5. 删除已完成的待办事项
```http
DELETE /api/v1/todos/completed
```

#### 6. 删除所有待办事项
```http
DELETE /api/v1/todos/all
```

## 数据库

### 表结构

```sql
CREATE TABLE todos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title VARCHAR(255) NOT NULL,
    description TEXT,
    completed BOOLEAN DEFAULT FALSE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

### 初始化数据

数据库会在首次启动时自动创建。你可以通过 API 接口添加测试数据。

## 测试

### 使用 curl 测试

```bash
# 获取所有待办事项
curl -X GET "http://localhost:8000/api/v1/todos"

# 创建待办事项
curl -X POST "http://localhost:8000/api/v1/todos" \
  -H "Content-Type: application/json" \
  -d '{"title": "测试待办事项", "description": "这是一个测试"}'

# 更新待办事项
curl -X PUT "http://localhost:8000/api/v1/todos/1" \
  -H "Content-Type: application/json" \
  -d '{"completed": true}'

# 删除待办事项
curl -X DELETE "http://localhost:8000/api/v1/todos/1"
```

### 使用 Python 测试

```python
import requests

# 测试创建待办事项
response = requests.post(
    "http://localhost:8000/api/v1/todos",
    json={"title": "Python 测试", "description": "使用 Python 测试 API"}
)
print(response.json())

# 测试获取待办事项
response = requests.get("http://localhost:8000/api/v1/todos")
print(response.json())
```

## 错误处理

API 使用统一的错误响应格式：

```json
{
  "code": 400,
  "message": "Validation error",
  "data": {
    "detail": "Title is required"
  }
}
```

## 开发说明

1. **数据库**: 使用 SQLite，数据库文件会自动创建在 `backend/database.db`
2. **CORS**: 已配置允许前端域名访问
3. **日志**: 使用 FastAPI 内置日志系统
4. **验证**: 使用 Pydantic 进行数据验证

## 部署

### 生产环境

```bash
# 安装生产依赖
pip install gunicorn

# 使用 Gunicorn 运行
gunicorn app.main:app -w 4 -k uvicorn.workers.UvicornWorker --bind 0.0.0.0:8000
```

### Docker 部署

```dockerfile
FROM python:3.9-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install -r requirements.txt

COPY . .
EXPOSE 8000

CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000"]
```
