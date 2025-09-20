from pydantic import BaseModel
from datetime import datetime
from typing import Optional

# 待办事项创建请求模型
class TodoCreate(BaseModel):
    title: str
    description: Optional[str] = None

# 待办事项更新请求模型
class TodoUpdate(BaseModel):
    title: Optional[str] = None
    description: Optional[str] = None
    completed: Optional[bool] = None

# 待办事项响应模型
class TodoResponse(BaseModel):
    id: int
    title: str
    description: Optional[str]
    completed: bool
    created_at: datetime
    updated_at: datetime
    
    class Config:
        from_attributes = True

# 统一响应模型
class ApiResponse(BaseModel):
    code: int
    message: str
    data: Optional[dict] = None

# 批量删除响应模型
class BatchDeleteResponse(BaseModel):
    code: int
    message: str
    data: dict
