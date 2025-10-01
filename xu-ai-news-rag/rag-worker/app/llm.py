import os
import requests
from typing import List

OLLAMA_ENDPOINT = os.getenv("OLLAMA_ENDPOINT", "http://localhost:11434")
MODEL_NAME = os.getenv("MODEL_NAME", "qwen2.5:3b")


def summarize(query: str, snippets: List[str]) -> str:
    context = "\n\n".join(snippets[:5])
    prompt = (
        "You are a helpful assistant. Based on the following context, answer the query in Chinese.\n"
        "If context is insufficient, say so briefly.\n\n"
        f"Query: {query}\n\nContext:\n{context}\n\nAnswer concisely:"
    )
    payload = {
        "model": MODEL_NAME,
        "prompt": prompt,
        "stream": False
    }
    try:
        resp = requests.post(f"{OLLAMA_ENDPOINT}/api/generate", json=payload, timeout=120)
        resp.raise_for_status()
        data = resp.json()
        return data.get("response", "")
    except Exception as e:
        return ""
