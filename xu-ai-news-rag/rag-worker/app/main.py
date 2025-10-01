from fastapi import FastAPI
from fastapi.responses import JSONResponse
from pydantic import BaseModel
from typing import List, Dict, Any, Optional

from .vector_store import store
from .llm import summarize as llm_summarize

app = FastAPI(title="xu-ai-news-rag-worker", version="0.1.0")


class UpsertItem(BaseModel):
    text: str
    metadata: Optional[Dict[str, Any]] = None
    id: Optional[str] = None


class UpsertRequest(BaseModel):
    items: List[UpsertItem]


class SearchRequest(BaseModel):
    query: str
    top_k: int = 5


@app.get("/health")
async def health():
    return JSONResponse({"status": "UP"})


@app.post("/upsert")
async def upsert(req: UpsertRequest):
    texts = [it.text for it in req.items]
    metadatas = [it.metadata or {} for it in req.items]
    ids = [it.id for it in req.items]
    store.upsert(texts, metadatas=metadatas, ids=ids)
    return {"inserted": len(texts)}


@app.post("/search")
async def search(req: SearchRequest):
    results = store.search(req.query, top_k=req.top_k)
    return {"results": results}


class SummarizeRequest(BaseModel):
    query: str
    snippets: List[str]


@app.post("/summarize")
async def summarize(req: SummarizeRequest):
    text = llm_summarize(req.query, req.snippets)
    return {"summary": text}
