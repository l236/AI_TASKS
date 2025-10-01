package com.xunews.backend.news;

import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface ChunkRepository extends JpaRepository<Chunk, Long> {
    List<Chunk> findByArticleId(Long articleId);
    List<Chunk> findTop1000ByOrderByIdDesc();
}
