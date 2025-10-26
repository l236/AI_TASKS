#include "SearchEngine.h"
#include <fstream>
#include <algorithm>

namespace FileSystemManager {

    SearchEngine::SearchEngine() : searchRoot(fs::current_path().string()), caseSensitive(false), useRegex(false) {
    }

    SearchEngine::SearchEngine(const std::string& rootPath) : searchRoot(rootPath), caseSensitive(false), useRegex(false) {
        if (!fs::exists(searchRoot)) {
            searchRoot = fs::current_path().string();
        }
    }

    void SearchEngine::setSearchRoot(const std::string& path) {
        if (fs::exists(path) && fs::is_directory(path)) {
            searchRoot = path;
        }
    }

    void SearchEngine::setCaseSensitive(bool sensitive) {
        caseSensitive = sensitive;
    }

    void SearchEngine::setUseRegex(bool useRegex) {
        this->useRegex = useRegex;
    }

    std::vector<SearchResult> SearchEngine::searchByName(const std::string& pattern, bool recursive) {
        searchStartTime = std::chrono::steady_clock::now();
        lastResults.clear();
        
        try {
            if (recursive) {
                searchByNameRecursive(searchRoot, pattern);
            } else {
                searchByNameInDirectory(searchRoot, pattern);
            }
        } catch (const std::exception&) {
            // Error handling
        }
        
        auto endTime = std::chrono::steady_clock::now();
        lastSearchStats.searchTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - searchStartTime);
        lastSearchStats.filesSearched = lastResults.size();
        lastSearchStats.filesMatched = lastResults.size();
        lastSearchStats.searchPattern = pattern;
        
        return lastResults;
    }

    void SearchEngine::searchByNameRecursive(const std::string& directory, const std::string& pattern) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (fs::is_regular_file(entry)) {
                    std::string fileName = entry.path().filename().string();
                    if (matchesFileName(fileName, pattern)) {
                        SearchResult result;
                        result.filePath = entry.path().string();
                        result.fileName = fileName;
                        result.fileSize = fs::file_size(entry);
                        
                        auto ftime = fs::last_write_time(entry);
                        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                        result.lastModified = formatTimestamp(sctp);
                        
                        lastResults.push_back(result);
                    }
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    void SearchEngine::searchByNameInDirectory(const std::string& directory, const std::string& pattern) {
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (fs::is_regular_file(entry)) {
                    std::string fileName = entry.path().filename().string();
                    if (matchesFileName(fileName, pattern)) {
                        SearchResult result;
                        result.filePath = entry.path().string();
                        result.fileName = fileName;
                        result.fileSize = fs::file_size(entry);
                        
                        auto ftime = fs::last_write_time(entry);
                        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                        result.lastModified = formatTimestamp(sctp);
                        
                        lastResults.push_back(result);
                    }
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    bool SearchEngine::matchesFileName(const std::string& fileName, const std::string& pattern) {
        if (useRegex) {
            try {
                std::regex regex(pattern, caseSensitive ? std::regex_constants::ECMAScript : 
                                               std::regex_constants::ECMAScript | std::regex_constants::icase);
                return std::regex_match(fileName, regex);
            } catch (const std::exception&) {
                return false;
            }
        } else {
            return matchesPattern(fileName, pattern);
        }
    }

    std::vector<SearchResult> SearchEngine::searchByExtension(const std::string& extension, bool recursive) {
        std::string pattern = "*" + (extension[0] == '.' ? extension : "." + extension);
        return searchByName(pattern, recursive);
    }

    std::vector<SearchResult> SearchEngine::searchBySize(size_t minSize, size_t maxSize, bool recursive) {
        searchStartTime = std::chrono::steady_clock::now();
        lastResults.clear();
        
        try {
            if (recursive) {
                searchBySizeRecursive(searchRoot, minSize, maxSize);
            } else {
                searchBySizeInDirectory(searchRoot, minSize, maxSize);
            }
        } catch (const std::exception&) {
            // Error handling
        }
        
        auto endTime = std::chrono::steady_clock::now();
        lastSearchStats.searchTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - searchStartTime);
        lastSearchStats.filesSearched = lastResults.size();
        lastSearchStats.filesMatched = lastResults.size();
        lastSearchStats.searchPattern = "size:" + std::to_string(minSize) + "-" + std::to_string(maxSize);
        
        return lastResults;
    }

    void SearchEngine::searchBySizeRecursive(const std::string& directory, size_t minSize, size_t maxSize) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (fs::is_regular_file(entry)) {
                    size_t fileSize = fs::file_size(entry);
                    if (fileSize >= minSize && fileSize <= maxSize) {
                        SearchResult result;
                        result.filePath = entry.path().string();
                        result.fileName = entry.path().filename().string();
                        result.fileSize = fileSize;
                        
                        auto ftime = fs::last_write_time(entry);
                        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                        result.lastModified = formatTimestamp(sctp);
                        
                        lastResults.push_back(result);
                    }
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    void SearchEngine::searchBySizeInDirectory(const std::string& directory, size_t minSize, size_t maxSize) {
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (fs::is_regular_file(entry)) {
                    size_t fileSize = fs::file_size(entry);
                    if (fileSize >= minSize && fileSize <= maxSize) {
                        SearchResult result;
                        result.filePath = entry.path().string();
                        result.fileName = entry.path().filename().string();
                        result.fileSize = fileSize;
                        
                        auto ftime = fs::last_write_time(entry);
                        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                        result.lastModified = formatTimestamp(sctp);
                        
                        lastResults.push_back(result);
                    }
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    std::vector<SearchResult> SearchEngine::searchInContent(const std::string& searchTerm, bool recursive) {
        searchStartTime = std::chrono::steady_clock::now();
        lastResults.clear();
        
        try {
            if (recursive) {
                searchInContentRecursive(searchRoot, searchTerm);
            } else {
                searchInContentInDirectory(searchRoot, searchTerm);
            }
        } catch (const std::exception&) {
            // Error handling
        }
        
        auto endTime = std::chrono::steady_clock::now();
        lastSearchStats.searchTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - searchStartTime);
        lastSearchStats.filesSearched = lastResults.size();
        lastSearchStats.filesMatched = lastResults.size();
        lastSearchStats.searchPattern = "content:" + searchTerm;
        
        return lastResults;
    }

    void SearchEngine::searchInContentRecursive(const std::string& directory, const std::string& searchTerm) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (fs::is_regular_file(entry) && !isBinaryFile(entry.path().string())) {
                    if (matchesFileContent(entry.path().string(), searchTerm)) {
                        SearchResult result;
                        result.filePath = entry.path().string();
                        result.fileName = entry.path().filename().string();
                        result.fileSize = fs::file_size(entry);
                        
                        auto ftime = fs::last_write_time(entry);
                        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                        result.lastModified = formatTimestamp(sctp);
                        
                        result.matchingLines = findMatchingLines(entry.path().string(), searchTerm);
                        lastResults.push_back(result);
                    }
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    void SearchEngine::searchInContentInDirectory(const std::string& directory, const std::string& searchTerm) {
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (fs::is_regular_file(entry) && !isBinaryFile(entry.path().string())) {
                    if (matchesFileContent(entry.path().string(), searchTerm)) {
                        SearchResult result;
                        result.filePath = entry.path().string();
                        result.fileName = entry.path().filename().string();
                        result.fileSize = fs::file_size(entry);
                        
                        auto ftime = fs::last_write_time(entry);
                        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                        result.lastModified = formatTimestamp(sctp);
                        
                        result.matchingLines = findMatchingLines(entry.path().string(), searchTerm);
                        lastResults.push_back(result);
                    }
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    bool SearchEngine::matchesFileContent(const std::string& filePath, const std::string& searchTerm) {
        try {
            std::ifstream file(filePath);
            if (!file.is_open()) return false;
            
            std::string line;
            std::string search = caseSensitive ? searchTerm : toLowerCase(searchTerm);
            
            while (std::getline(file, line)) {
                std::string lineToSearch = caseSensitive ? line : toLowerCase(line);
                if (lineToSearch.find(search) != std::string::npos) {
                    file.close();
                    return true;
                }
            }
            file.close();
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    std::vector<std::string> SearchEngine::findMatchingLines(const std::string& filePath, const std::string& searchTerm) {
        std::vector<std::string> matchingLines;
        try {
            std::ifstream file(filePath);
            if (!file.is_open()) return matchingLines;
            
            std::string line;
            std::string search = caseSensitive ? searchTerm : toLowerCase(searchTerm);
            int lineNumber = 1;
            
            while (std::getline(file, line)) {
                std::string lineToSearch = caseSensitive ? line : toLowerCase(line);
                if (lineToSearch.find(search) != std::string::npos) {
                    matchingLines.push_back("Line " + std::to_string(lineNumber) + ": " + line);
                }
                lineNumber++;
            }
            file.close();
        } catch (const std::exception&) {
            // Error handling
        }
        return matchingLines;
    }

    bool SearchEngine::isBinaryFile(const std::string& filePath) {
        try {
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) return true;
            
            char buffer[1024];
            file.read(buffer, sizeof(buffer));
            size_t bytesRead = file.gcount();
            
            for (size_t i = 0; i < bytesRead; ++i) {
                if (buffer[i] == 0) {
                    return true; // Binary file
                }
            }
            return false;
        } catch (const std::exception&) {
            return true;
        }
    }

    std::vector<SearchResult> SearchEngine::searchInContent(const std::string& searchTerm, const std::string& fileExtension, bool recursive) {
        auto results = searchInContent(searchTerm, recursive);
        std::vector<SearchResult> filteredResults;
        
        std::string ext = fileExtension;
        if (!ext.empty() && ext[0] != '.') {
            ext = "." + ext;
        }
        
        std::copy_if(results.begin(), results.end(), std::back_inserter(filteredResults),
            [&ext](const SearchResult& result) {
                return fs::path(result.filePath).extension().string() == ext;
            });
        
        return filteredResults;
    }

    std::vector<SearchResult> SearchEngine::getLastResults() const {
        return lastResults;
    }

    void SearchEngine::clearResults() {
        lastResults.clear();
    }

    size_t SearchEngine::getResultCount() const {
        return lastResults.size();
    }

    SearchEngine::SearchStats SearchEngine::getLastSearchStats() const {
        return lastSearchStats;
    }

    std::future<std::vector<SearchResult>> SearchEngine::searchByNameAsync(const std::string& pattern, bool recursive) {
        return std::async(std::launch::async, [this, pattern, recursive]() {
            return searchByName(pattern, recursive);
        });
    }

    std::future<std::vector<SearchResult>> SearchEngine::searchInContentAsync(const std::string& searchTerm, bool recursive) {
        return std::async(std::launch::async, [this, searchTerm, recursive]() {
            return searchInContent(searchTerm, recursive);
        });
    }

}

