# xu-ai-news-rag

个性化新闻智能知识库（RAG）

## 快速开始

- 一键启动（开发）：

```bash
# 首次启动会自动拉取并运行 mysql、ollama、rag-worker、backend
docker compose up -d --build
# 拉取模型（首次执行）
docker exec -it xu-ollama ollama pull qwen2.5:3b
```

- 健康检查：
  - Backend: http://localhost:8080/actuator/health 或 http://localhost:8080/health
  - Swagger: http://localhost:8080/swagger-ui.html
  - RAG Worker: http://localhost:8001/health
  - Ollama: http://localhost:11434/api/tags

## 不用 Docker 的本地运行

- rag-worker：
  ```bash
  python3 -m venv .venv
  source .venv/bin/activate
  pip install -r rag-worker/requirements.txt
  uvicorn app.main:app --host 0.0.0.0 --port 8001 --reload --app-dir rag-worker
  ```
- Ollama（可选，用于 /summarize）：
  ```bash
  # 安装后
  ollama serve
  ollama pull qwen2.5:3b
  ```
- 后端（H2 dev 配置）：
  ```bash
  mvn -q -f backend/pom.xml spring-boot:run -Dspring-boot.run.profiles=dev
  ```

## 目录结构

- backend/  Spring Boot 后端
- rag-worker/  Python FastAPI + FAISS 检索 + Ollama 总结
- docker-compose.yml  编排 MySQL、Ollama、RAG、后端

## 配置

- `backend/src/main/resources/application-dev.properties`
  - H2 内存库、Swagger、H2 Console
- 运行时环境变量（compose 中已注入）：
  - `RAG_WORKER_URL`（默认 http://rag-worker:8001 或本地 http://localhost:8001）
  - `app.search.baidu.enabled=true` 启用兜底百度搜索
- RSS（示例，添加到 `application-dev.properties`）：
  ```properties
  rss.feeds[0]=https://example.com/rss.xml
  rss.interval-minutes=60
  ```

## API 使用示例

- 注册与登录
  ```bash
  curl -s -X POST http://localhost:8080/api/auth/register \
    -H "Content-Type: application/json" \
    -d '{"username":"user1","password":"pass123","email":"u1@example.com"}'

  curl -s -X POST http://localhost:8080/api/auth/login \
    -H "Content-Type: application/json" \
    -d '{"username":"user1","password":"pass123"}'
  ```

- 文本入库
  ```bash
  TOKEN=... # 上一步获取
  curl -s -X POST http://localhost:8080/api/rag/ingest/text \
    -H "Authorization: Bearer $TOKEN" -H "Content-Type: application/json" \
    -d '{
      "sourceName":"Demo",
      "title":"AI News",
      "content":"OpenAI and Qwen release new models..."
    }'
  ```

- 检索（RAG 命中返回匹配片段；未命中触发百度 Top3 + 总结）
  ```bash
  curl -s -X POST http://localhost:8080/api/rag/search \
    -H "Authorization: Bearer $TOKEN" -H "Content-Type: application/json" \
    -d '{"query":"new models", "topK":5}'
  ```

- Excel 导入（列：sourceName, sourceUrl, title, url, content）
  ```bash
  curl -s -X POST http://localhost:8080/api/rag/ingest/excel \
    -H "Authorization: Bearer $TOKEN" \
    -F file=@samples/news.xlsx
  ```

- 关键词 Top10
  ```bash
  curl -s -X GET http://localhost:8080/api/analytics/keywords/top10 \
    -H "Authorization: Bearer $TOKEN"
  ```

## 测试

```bash
mvn -q -f backend/pom.xml test
```
覆盖测试：
- Auth：注册/登录
- RAG：文本入库 + 检索（mock rag-worker）
- Analytics：Top10 关键词

## 里程碑（进度）

- [x] 脚手架与健康检查
- [x] JWT 登录注册（Spring Security）
- [x] 文本入库与检索（FAISS 向量 + 兜底总结）
- [x] Excel 导入
- [x] RSS 抓取与调度（去重、正文提取）
- [x] 关键词统计（Top10）
- [x] 后端测试（MockMvc/H2）
- [ ] 文档完善（PRD/架构/原型）
- [ ] 前端（React + AntD）

## 抓取合规说明（重要）

- 遵守目标站点 `robots.txt` 协议：仅抓取允许的路径；若禁爬则不抓取。
- 合理设置 `User-Agent` 与超时/重试策略，避免对站点造成过载；默认抓取间隔≥300ms。
- 仅采集公开可访问的信息；不绕过任何认证、验证码、反爬等安全措施。
- 保留并标注来源链接与标题，仅用于学习与研究目的；如需商用或再分发，请遵循源站授权条款。
- 如版权方或站点提出异议，请及时删除相关内容并停止抓取。

## 许可证
MIT
