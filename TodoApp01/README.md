# TodoApp01 - 全栈待办事项应用

一个基于 FastAPI + React + SQLite 的全栈待办事项应用，支持完整的 CRUD 操作和现代化 UI 设计。

## 🚀 项目特性

- **全栈架构**: 前后端分离，RESTful API 设计
- **现代化技术栈**: FastAPI + React + TypeScript + SQLite
- **完整功能**: 增删改查、过滤、批量操作
- **响应式设计**: 支持桌面端和移动端
- **实时同步**: 前后端数据实时同步
- **类型安全**: TypeScript 提供完整的类型检查

## 📁 项目结构

```
TodoApp01/
├── backend/                 # 后端服务
│   ├── app/
│   │   ├── main.py         # FastAPI 应用入口
│   │   ├── database.py     # 数据库配置
│   │   ├── schemas.py      # 数据模型
│   │   └── routers/
│   │       └── todos.py    # 待办事项 API
│   ├── requirements.txt    # Python 依赖
│   └── README.md          # 后端说明
├── frontend/               # 前端应用
│   ├── src/
│   │   ├── components/     # React 组件
│   │   ├── services/       # API 服务
│   │   └── types/          # TypeScript 类型
│   ├── package.json        # 前端依赖
│   └── README.md          # 前端说明
├── req.md                 # 需求文档
├── TECH_ARCHITECTURE.md   # 技术架构文档
└── README.md             # 项目说明
```

## 🛠️ 技术栈

### 后端
- **FastAPI**: 现代、快速的 Web 框架
- **SQLAlchemy**: Python ORM
- **SQLite**: 轻量级数据库
- **Pydantic**: 数据验证
- **Uvicorn**: ASGI 服务器

### 前端
- **React 18**: 用户界面库
- **TypeScript**: 类型安全
- **Axios**: HTTP 客户端
- **CSS3**: 现代化样式设计

## 🚀 快速开始

### 环境要求
- Python 3.8+
- Node.js 16+
- npm 或 yarn

### 1. 启动后端服务

```bash
cd backend
pip install -r requirements.txt
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```

后端服务将在 http://localhost:8000 启动

### 2. 启动前端应用

```bash
cd frontend
npm install
npm start
```

前端应用将在 http://localhost:3000 启动

### 3. 访问应用

- **前端应用**: http://localhost:3000
- **API 文档**: http://localhost:8000/docs
- **ReDoc 文档**: http://localhost:8000/redoc

## 📋 功能列表

### 基础功能
- ✅ 添加待办事项（标题 + 描述）
- ✅ 查看待办事项列表
- ✅ 编辑待办事项
- ✅ 删除待办事项
- ✅ 标记完成/未完成

### 高级功能
- ✅ 过滤显示（全部/未完成/已完成）
- ✅ 批量删除（清除已完成/清除全部）
- ✅ 实时数据同步
- ✅ 响应式设计
- ✅ 错误处理
- ✅ 加载状态

### UI/UX 特性
- ✅ 现代化设计风格
- ✅ 渐变背景和动画效果
- ✅ 悬停交互反馈
- ✅ 移动端适配
- ✅ 空状态提示
- ✅ 确认对话框

## 🔧 API 接口

### 基础信息
- **Base URL**: `http://localhost:8000/api/v1`
- **Content-Type**: `application/json`

### 主要接口

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/todos/` | 获取待办事项列表 |
| POST | `/todos/` | 创建待办事项 |
| PUT | `/todos/{id}/` | 更新待办事项 |
| DELETE | `/todos/{id}/` | 删除待办事项 |
| DELETE | `/todos/completed/` | 删除已完成的待办事项 |
| DELETE | `/todos/all/` | 删除所有待办事项 |

### 查询参数
- `status`: `all` | `active` | `completed` - 过滤状态

## 🗄️ 数据库设计

### todos 表
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

## 🎨 设计特色

### 视觉设计
- **渐变背景**: 蓝紫色渐变营造现代感
- **卡片设计**: 圆角卡片提升视觉层次
- **动画效果**: 悬停和点击动画增强交互体验
- **色彩系统**: 统一的色彩规范

### 交互设计
- **即时反馈**: 操作后立即显示结果
- **确认机制**: 危险操作需要用户确认
- **状态指示**: 清晰的加载和错误状态
- **响应式布局**: 适配不同屏幕尺寸

## 🧪 测试

### 后端测试
```bash
cd backend
# 使用 curl 测试 API
curl -X GET "http://localhost:8000/api/v1/todos/"
curl -X POST "http://localhost:8000/api/v1/todos/" \
  -H "Content-Type: application/json" \
  -d '{"title": "测试待办事项", "description": "这是一个测试"}'
```

### 前端测试
```bash
cd frontend
npm test
```

## 📦 部署

### 开发环境
按照"快速开始"部分的说明启动服务

### 生产环境

#### 后端部署
```bash
# 使用 Gunicorn
pip install gunicorn
gunicorn app.main:app -w 4 -k uvicorn.workers.UvicornWorker --bind 0.0.0.0:8000
```

#### 前端部署
```bash
npm run build
# 将 build/ 目录部署到静态文件服务器
```

### Docker 部署
```dockerfile
# 后端 Dockerfile
FROM python:3.9-slim
WORKDIR /app
COPY requirements.txt .
RUN pip install -r requirements.txt
COPY . .
EXPOSE 8000
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000"]

# 前端 Dockerfile
FROM node:16-alpine as build
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build

FROM nginx:alpine
COPY --from=build /app/build /usr/share/nginx/html
EXPOSE 80
```

## 🤝 贡献指南

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 Pull Request

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情

## 📞 联系方式

如有问题或建议，请通过以下方式联系：

- 项目 Issues: [GitHub Issues](https://github.com/your-username/TodoApp01/issues)
- 邮箱: your-email@example.com

## 🙏 致谢

感谢以下开源项目：
- [FastAPI](https://fastapi.tiangolo.com/)
- [React](https://reactjs.org/)
- [SQLAlchemy](https://www.sqlalchemy.org/)
- [TypeScript](https://www.typescriptlang.org/)
