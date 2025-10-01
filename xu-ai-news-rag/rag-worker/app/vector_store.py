import os
from typing import List, Dict, Any, Optional

import numpy as np
from sentence_transformers import SentenceTransformer
try:
    import faiss  # type: ignore
    FAISS_AVAILABLE = True
except Exception:
    faiss = None  # type: ignore
    FAISS_AVAILABLE = False


class VectorStore:
    def __init__(self, model_name: str = "sentence-transformers/all-MiniLM-L6-v2"):
        self.model = SentenceTransformer(model_name)
        self.dim = self.model.get_sentence_embedding_dimension()
        self.index = faiss.IndexFlatIP(self.dim) if FAISS_AVAILABLE else None
        self.ids: List[str] = []
        self.metadatas: List[Dict[str, Any]] = []
        self.texts: List[str] = []
        # Fallback: keep all embeddings in memory for NumPy search
        self._embs: Optional[np.ndarray] = None  # shape (N, dim), L2-normalized

    def _embed(self, texts: List[str]) -> np.ndarray:
        embs = self.model.encode(texts, normalize_embeddings=True)
        return np.asarray(embs, dtype="float32")

    def upsert(self, texts: List[str], metadatas: Optional[List[Dict[str, Any]]] = None, ids: Optional[List[str]] = None):
        if metadatas is None:
            metadatas = [{} for _ in texts]
        if ids is None:
            ids = [str(len(self.ids) + i) for i in range(len(texts))]
        embs = self._embed(texts)
        if FAISS_AVAILABLE and self.index is not None:
            self.index.add(embs)
        # maintain numpy matrix
        if self._embs is None:
            self._embs = embs
        else:
            self._embs = np.vstack([self._embs, embs])
        self.texts.extend(texts)
        self.metadatas.extend(metadatas)
        self.ids.extend(ids)

    def search(self, query: str, top_k: int = 5):
        if len(self.ids) == 0:
            return []
        q = self._embed([query])
        k = min(top_k, len(self.ids))
        if FAISS_AVAILABLE and self.index is not None:
            scores, idxs = self.index.search(q, k)
            idx_list = idxs[0]
            score_list = scores[0]
        else:
            # cosine similarity == dot product since embeddings are normalized
            if self._embs is None or self._embs.shape[0] == 0:
                return []
            sims = (q @ self._embs.T)[0]  # shape (N,)
            idx_list = np.argsort(-sims)[:k]
            score_list = sims[idx_list]
        results = []
        for score, idx in zip(score_list, idx_list):
            if idx == -1:
                continue
            results.append({
                "id": self.ids[idx],
                "text": self.texts[idx],
                "metadata": self.metadatas[idx],
                "score": float(score)
            })
        return results


# Singleton store
store = VectorStore()
