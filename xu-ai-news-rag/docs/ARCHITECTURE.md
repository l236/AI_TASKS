# 架构设计文档

## 1. 总览

系统采用前后端分离 + RAG Worker 的多服务架构，支持 Docker Compose 一键编排。

- 后端（Spring Boot 3）：鉴权、采集、入库、检索 API、RSS 调度、分析。
- RAG Worker（FastAPI）：嵌入、FAISS 检索、LLM 总结。
- 向量库：FAISS（容器卷持久化）。
- 关系库：MySQL（dev 使用 H2）。
- LLM：Ollama 本地 `qwen2.5:3b`。

## 2. 组件与职责

- `backend/`
  - `auth/*`：用户注册/登录，JWT 发放与校验。
  - `news/*`：`Source`/`Article`/`Chunk` 实体与仓储、入库逻辑。
  - `rag/*`：与 RAG Worker 通信（`RagClient`），入库/检索 `RagService` 与控制器。
  - `rss/*`：RSS 定时任务（Rome 解析）、Jsoup 正文抽取、去重策略。
  - `analytics/*`：关键词统计（Top10）。
  - `config/*`：通用 Bean，如 `RestTemplate`。
- `rag-worker/`
  - `vector_store.py`：Sentence-Transformers 嵌入，FAISS 相似度检索。
  - `llm.py`：调用 Ollama 进行总结。
  - `main.py`：`/upsert`、`/search`、`/summarize` 接口。

## 3. 时序流程

- 入库（文本/Excel/RSS）：
  1. 后端持久化元数据（MySQL/H2），分块生成 `Chunk`。
  2. 调用 RAG Worker `/upsert` 写入向量库（FAISS）。
- 检索：
  1. 调用 RAG Worker `/search` 获取 TopK 片段。
  2. 若命中为空，触发百度 Top3 兜底，调用 `/summarize` 生成总结。

## 4. 数据模型（简化）

- `users(id, username, email, password_hash, role, created_at)`
- `sources(id, name, url, created_at)`
- `articles(id, source_id, title, url, content, published_at, created_at)`
- `chunks(id, article_id, text, vector_id, created_at)`

## 5. 关键技术决策

- 嵌入模型：`all-MiniLM-L6-v2`（轻量、速度优先）。
- 重排：`ms-marco-MiniLM-L-6-v2` 暂留接口，后续接入。
- LLM：Ollama `qwen2.5:3b` 本地化运行，降低外部依赖。
- 合规抓取：robots、User-Agent、超时与限速（基础版）。

## 6. 扩展性与替换点

- 向量库可替换为 Milvus/PGVecto.rs；
- 嵌入/重排/LLM 模型可配置；
- RSS/抓取可引入更强正文抽取（readability/boilerpipe）。

## 7. 运维与观测

- Actuator 健康检查；
- Swagger/OpenAPI 文档；
- 通过 Docker Compose 编排 MySQL/Ollama/Worker/Backend。
