from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from .database import create_tables
from .routers import todos

# 创建FastAPI应用
app = FastAPI(
    title="Todo App API",
    description="A simple todo application API",
    version="1.0.0"
)

# 配置CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000", "http://127.0.0.1:3000"],  # 前端地址
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 创建数据库表
create_tables()

# 注册路由
app.include_router(todos.router, prefix="/api/v1")

@app.get("/")
async def root():
    """根路径"""
    return {"message": "Todo App API is running!"}

@app.get("/health")
async def health_check():
    """健康检查"""
    return {"status": "healthy", "message": "API is working properly"}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
