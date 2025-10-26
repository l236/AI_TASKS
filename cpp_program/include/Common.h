#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

// Common utility functions and types
namespace FileSystemManager {

    // File information structure
    struct FileInfo {
        std::string name;
        std::string path;
        std::string extension;
        size_t size;
        std::string lastModified;
        bool isDirectory;
        
        FileInfo() = default;
        FileInfo(const fs::path& filePath);
    };

    // Search result structure
    struct SearchResult {
        std::string filePath;
        std::string fileName;
        size_t fileSize;
        std::string lastModified;
        std::vector<std::string> matchingLines; // For content search
    };

    // Operation result structure
    struct OperationResult {
        bool success;
        std::string message;
        size_t filesProcessed;
        size_t filesSkipped;
        std::vector<std::string> errors;
    };

    // Utility functions
    std::string formatFileSize(size_t bytes);
    std::string getCurrentTimestamp();
    std::string formatTimestamp(const std::chrono::system_clock::time_point& time);
    bool isValidPath(const std::string& path);
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    std::string toLowerCase(const std::string& str);
    bool matchesPattern(const std::string& filename, const std::string& pattern);
}

