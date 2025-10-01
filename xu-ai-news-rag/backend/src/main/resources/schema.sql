-- MySQL schema for xu-ai-news-rag
-- Run against a fresh database (xunews). Adjust engine/charset as needed.

CREATE TABLE IF NOT EXISTS users (
  id BIGINT PRIMARY KEY AUTO_INCREMENT,
  username VARCHAR(50) NOT NULL UNIQUE,
  email VARCHAR(100),
  password_hash VARCHAR(255) NOT NULL,
  role VARCHAR(20) NOT NULL,
  created_at DATETIME NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS sources (
  id BIGINT PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(200) NOT NULL,
  url VARCHAR(500),
  created_at DATETIME NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS articles (
  id BIGINT PRIMARY KEY AUTO_INCREMENT,
  source_id BIGINT,
  title VARCHAR(500) NOT NULL,
  url VARCHAR(1000),
  content LONGTEXT,
  published_at DATETIME NULL,
  created_at DATETIME NOT NULL,
  INDEX idx_articles_source (source_id),
  CONSTRAINT fk_articles_source FOREIGN KEY (source_id)
    REFERENCES sources(id) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS chunks (
  id BIGINT PRIMARY KEY AUTO_INCREMENT,
  article_id BIGINT,
  text LONGTEXT,
  vector_id VARCHAR(100),
  created_at DATETIME NOT NULL,
  INDEX idx_chunks_article (article_id),
  CONSTRAINT fk_chunks_article FOREIGN KEY (article_id)
    REFERENCES articles(id) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
