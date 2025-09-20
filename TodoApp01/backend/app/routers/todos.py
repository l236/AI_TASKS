from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session
from typing import List, Optional
from datetime import datetime

from ..database import get_db, Todo
from ..schemas import TodoCreate, TodoUpdate, TodoResponse, ApiResponse, BatchDeleteResponse

router = APIRouter(prefix="/todos", tags=["todos"])

@router.get("/", response_model=ApiResponse)
async def get_todos(
    status: Optional[str] = Query("all", description="Filter by status: all, active, completed"),
    db: Session = Depends(get_db)
):
    """获取所有待办事项"""
    try:
        query = db.query(Todo)
        
        if status == "active":
            query = query.filter(Todo.completed == False)
        elif status == "completed":
            query = query.filter(Todo.completed == True)
        
        todos = query.order_by(Todo.created_at.desc()).all()
        
        return ApiResponse(
            code=200,
            message="success",
            data={"todos": [TodoResponse.from_orm(todo) for todo in todos]}
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@router.post("/", response_model=ApiResponse)
async def create_todo(todo: TodoCreate, db: Session = Depends(get_db)):
    """创建新的待办事项"""
    try:
        db_todo = Todo(
            title=todo.title,
            description=todo.description,
            completed=False
        )
        db.add(db_todo)
        db.commit()
        db.refresh(db_todo)
        
        return ApiResponse(
            code=201,
            message="Todo created successfully",
            data={"todo": TodoResponse.from_orm(db_todo)}
        )
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@router.put("/{todo_id}", response_model=ApiResponse)
async def update_todo(
    todo_id: int, 
    todo: TodoUpdate, 
    db: Session = Depends(get_db)
):
    """更新待办事项"""
    try:
        db_todo = db.query(Todo).filter(Todo.id == todo_id).first()
        if not db_todo:
            raise HTTPException(status_code=404, detail="Todo not found")
        
        update_data = todo.dict(exclude_unset=True)
        for field, value in update_data.items():
            setattr(db_todo, field, value)
        
        db_todo.updated_at = datetime.utcnow()
        db.commit()
        db.refresh(db_todo)
        
        return ApiResponse(
            code=200,
            message="Todo updated successfully",
            data={"todo": TodoResponse.from_orm(db_todo)}
        )
    except HTTPException:
        raise
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@router.delete("/{todo_id}", response_model=ApiResponse)
async def delete_todo(todo_id: int, db: Session = Depends(get_db)):
    """删除待办事项"""
    try:
        db_todo = db.query(Todo).filter(Todo.id == todo_id).first()
        if not db_todo:
            raise HTTPException(status_code=404, detail="Todo not found")
        
        db.delete(db_todo)
        db.commit()
        
        return ApiResponse(
            code=200,
            message="Todo deleted successfully",
            data=None
        )
    except HTTPException:
        raise
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@router.delete("/completed", response_model=BatchDeleteResponse)
async def delete_completed_todos(db: Session = Depends(get_db)):
    """删除所有已完成的待办事项"""
    try:
        completed_todos = db.query(Todo).filter(Todo.completed == True).all()
        deleted_count = len(completed_todos)
        
        for todo in completed_todos:
            db.delete(todo)
        
        db.commit()
        
        return BatchDeleteResponse(
            code=200,
            message="Completed todos deleted successfully",
            data={"deleted_count": deleted_count}
        )
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@router.delete("/all", response_model=BatchDeleteResponse)
async def delete_all_todos(db: Session = Depends(get_db)):
    """删除所有待办事项"""
    try:
        all_todos = db.query(Todo).all()
        deleted_count = len(all_todos)
        
        for todo in all_todos:
            db.delete(todo)
        
        db.commit()
        
        return BatchDeleteResponse(
            code=200,
            message="All todos deleted successfully",
            data={"deleted_count": deleted_count}
        )
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=500, detail=str(e))
