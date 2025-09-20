# Todo App Frontend

基于 React + TypeScript 的待办事项应用前端。

## 技术栈

- **React 18**: 用户界面库
- **TypeScript**: 类型安全的 JavaScript
- **Axios**: HTTP 客户端
- **CSS3**: 样式设计
- **Create React App**: 项目脚手架

## 功能特性

- ✅ 添加待办事项（标题 + 描述）
- ✅ 标记完成/未完成
- ✅ 编辑待办事项
- ✅ 删除待办事项
- ✅ 过滤显示（全部/未完成/已完成）
- ✅ 批量删除（清除已完成/清除全部）
- ✅ 实时数据同步
- ✅ 响应式设计
- ✅ 现代化UI设计

## 项目结构

```
frontend/
├── public/
├── src/
│   ├── components/          # React 组件
│   │   ├── TodoApp.tsx     # 主应用组件
│   │   ├── TodoForm.tsx    # 添加待办事项表单
│   │   ├── TodoList.tsx    # 待办事项列表
│   │   ├── TodoItem.tsx    # 单个待办事项
│   │   ├── TodoFilters.tsx # 过滤器组件
│   │   ├── TodoActions.tsx # 操作按钮组件
│   │   └── *.css          # 组件样式文件
│   ├── services/           # API 服务
│   │   └── api.ts         # API 接口封装
│   ├── types/             # TypeScript 类型定义
│   │   └── todo.ts        # 待办事项类型
│   ├── App.tsx            # 根组件
│   ├── App.css            # 全局样式
│   └── index.tsx          # 应用入口
├── package.json           # 项目依赖
└── README.md             # 说明文档
```

## 安装和运行

### 1. 安装依赖

```bash
cd frontend
npm install
```

### 2. 启动开发服务器

```bash
npm start
```

应用将在 http://localhost:3000 打开

### 3. 构建生产版本

```bash
npm run build
```

## 组件说明

### TodoApp
主应用组件，负责状态管理和数据流控制。

**主要功能**:
- 管理待办事项状态
- 处理 API 调用
- 错误处理
- 统计信息计算

### TodoForm
添加待办事项的表单组件。

**主要功能**:
- 输入待办事项标题
- 输入详细描述（可选）
- 表单验证
- 提交处理

### TodoList
待办事项列表容器组件。

**主要功能**:
- 显示待办事项列表
- 加载状态显示
- 空状态提示

### TodoItem
单个待办事项组件。

**主要功能**:
- 显示待办事项信息
- 完成/未完成切换
- 编辑功能
- 删除功能
- 时间显示

### TodoFilters
过滤器组件。

**主要功能**:
- 全部/未完成/已完成过滤
- 过滤状态管理

### TodoActions
批量操作组件。

**主要功能**:
- 清除已完成的待办事项
- 清除所有待办事项

## API 集成

前端通过 `services/api.ts` 与后端 API 进行通信：

```typescript
// 获取待办事项
const todos = await todoApi.getTodos('all');

// 创建待办事项
const newTodo = await todoApi.createTodo({
  title: '学习 React',
  description: '完成 React 基础教程'
});

// 更新待办事项
const updatedTodo = await todoApi.updateTodo(1, {
  completed: true
});

// 删除待办事项
await todoApi.deleteTodo(1);
```

## 样式设计

### 设计理念
- **现代化**: 使用渐变背景、圆角、阴影等现代设计元素
- **响应式**: 支持桌面端和移动端
- **交互反馈**: 悬停效果、按钮动画
- **视觉层次**: 清晰的信息架构和视觉引导

### 颜色方案
- **主色调**: 蓝紫渐变 (#667eea → #764ba2)
- **成功色**: 绿色 (#4caf50)
- **警告色**: 橙色 (#ff9800)
- **危险色**: 红色 (#f44336)
- **中性色**: 灰色系

### 响应式断点
- **桌面端**: > 768px
- **移动端**: ≤ 768px

## 开发说明

### 状态管理
使用 React Hooks 进行状态管理：
- `useState`: 本地状态
- `useEffect`: 副作用处理

### 错误处理
- API 调用错误捕获
- 用户友好的错误提示
- 网络连接检查

### 性能优化
- 组件懒加载
- 避免不必要的重渲染
- 合理的状态更新策略

## 浏览器支持

- Chrome 60+
- Firefox 55+
- Safari 12+
- Edge 79+

## 部署

### 开发环境
```bash
npm start
```

### 生产环境
```bash
npm run build
# 构建文件在 build/ 目录
```

### Docker 部署
```dockerfile
FROM node:16-alpine

WORKDIR /app
COPY package*.json ./
RUN npm install

COPY . .
RUN npm run build

FROM nginx:alpine
COPY --from=0 /app/build /usr/share/nginx/html
EXPOSE 80
```