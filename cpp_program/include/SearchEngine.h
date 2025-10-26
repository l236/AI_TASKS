#pragma once

#include "Common.h"
#include <regex>
#include <future>
#include <functional>

namespace FileSystemManager {

    class SearchEngine {
    private:
        std::string searchRoot;
        std::vector<SearchResult> lastResults;
        bool caseSensitive;
        bool useRegex;
        
        bool matchesFileName(const std::string& fileName, const std::string& pattern);
        bool matchesFileContent(const std::string& filePath, const std::string& searchTerm);
        std::vector<std::string> findMatchingLines(const std::string& filePath, const std::string& searchTerm);
        bool isBinaryFile(const std::string& filePath);
        
    public:
        SearchEngine();
        explicit SearchEngine(const std::string& rootPath);
        ~SearchEngine() = default;
        
        // Configuration
        void setSearchRoot(const std::string& path);
        void setCaseSensitive(bool sensitive);
        void setUseRegex(bool useRegex);
        
        // File name search
        std::vector<SearchResult> searchByName(const std::string& pattern, bool recursive = true);
        std::vector<SearchResult> searchByExtension(const std::string& extension, bool recursive = true);
        std::vector<SearchResult> searchBySize(size_t minSize, size_t maxSize = SIZE_MAX, bool recursive = true);
        std::vector<SearchResult> searchByDate(const std::string& startDate, const std::string& endDate = "", bool recursive = true);
        
        // Content search
        std::vector<SearchResult> searchInContent(const std::string& searchTerm, bool recursive = true);
        std::vector<SearchResult> searchInContent(const std::string& searchTerm, const std::string& fileExtension, bool recursive = true);
        
        // Advanced search
        std::vector<SearchResult> searchAdvanced(const std::string& namePattern, const std::string& contentTerm, 
                                                const std::string& extension = "", size_t minSize = 0, 
                                                size_t maxSize = SIZE_MAX, bool recursive = true);
        
        // Duplicate file detection
        std::vector<std::vector<SearchResult>> findDuplicateFiles(bool byContent = true);
        std::vector<std::vector<SearchResult>> findDuplicateFilesBySize();
        
        // Search utilities
        std::vector<SearchResult> getLastResults() const;
        void clearResults();
        size_t getResultCount() const;
        
        // Async search (for large directories)
        std::future<std::vector<SearchResult>> searchByNameAsync(const std::string& pattern, bool recursive = true);
        std::future<std::vector<SearchResult>> searchInContentAsync(const std::string& searchTerm, bool recursive = true);
        
        // Search statistics
        struct SearchStats {
            size_t filesSearched;
            size_t filesMatched;
            std::chrono::milliseconds searchTime;
            std::string searchPattern;
        };
        
        SearchStats getLastSearchStats() const;
        
    private:
        void searchByNameRecursive(const std::string& directory, const std::string& pattern);
        void searchByNameInDirectory(const std::string& directory, const std::string& pattern);
        void searchBySizeRecursive(const std::string& directory, size_t minSize, size_t maxSize);
        void searchBySizeInDirectory(const std::string& directory, size_t minSize, size_t maxSize);
        void searchInContentRecursive(const std::string& directory, const std::string& searchTerm);
        void searchInContentInDirectory(const std::string& directory, const std::string& searchTerm);
        
        SearchStats lastSearchStats;
        std::chrono::steady_clock::time_point searchStartTime;
    };

}
