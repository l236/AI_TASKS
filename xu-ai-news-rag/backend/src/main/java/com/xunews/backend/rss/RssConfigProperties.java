package com.xunews.backend.rss;

import lombok.Getter;
import lombok.Setter;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;

import java.util.ArrayList;
import java.util.List;

@Configuration
@ConfigurationProperties(prefix = "rss")
@Getter
@Setter
public class RssConfigProperties {
    /**
     * Feed URLs, e.g. https://example.com/rss.xml
     */
    private List<String> feeds = new ArrayList<>();

    /**
     * Fetch interval minutes
     */
    private int intervalMinutes = 60;
}
