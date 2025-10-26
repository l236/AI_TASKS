#pragma once

#include "Common.h"
#include <map>
#include <set>

namespace FileSystemManager {

    class FileManager {
    private:
        std::string currentPath;
        std::map<std::string, std::vector<FileInfo>> directoryCache;
        std::set<std::string> watchedDirectories;
        
        void updateCache(const std::string& path);
        bool isTextFile(const std::string& filePath);
        
    public:
        FileManager();
        explicit FileManager(const std::string& initialPath);
        ~FileManager() = default;
        
        // Navigation
        bool changeDirectory(const std::string& path);
        std::string getCurrentPath() const;
        std::string getParentDirectory() const;
        
        // File listing
        std::vector<FileInfo> listFiles(bool includeHidden = false);
        std::vector<FileInfo> listFilesByExtension(const std::string& extension);
        std::vector<FileInfo> listDirectories();
        
        // File operations
        bool createFile(const std::string& fileName, const std::string& content = "");
        bool createDirectory(const std::string& dirName);
        bool deleteFile(const std::string& fileName);
        bool deleteDirectory(const std::string& dirName);
        bool copyFile(const std::string& source, const std::string& destination);
        bool moveFile(const std::string& source, const std::string& destination);
        bool renameFile(const std::string& oldName, const std::string& newName);
        
        // File information
        FileInfo getFileInfo(const std::string& fileName);
        bool fileExists(const std::string& fileName);
        bool directoryExists(const std::string& dirName);
        size_t getDirectorySize(const std::string& dirPath);
        
        // File content operations
        std::string readFileContent(const std::string& fileName);
        bool writeFileContent(const std::string& fileName, const std::string& content);
        bool appendToFile(const std::string& fileName, const std::string& content);
        
        // Directory management
        void refreshCache();
        void clearCache();
        std::vector<std::string> getDirectoryTree(const std::string& path = "", int maxDepth = 3);
        
        // File watching (basic implementation)
        void addWatchedDirectory(const std::string& path);
        void removeWatchedDirectory(const std::string& path);
        std::vector<std::string> getChangedFiles();
        
    private:
        void getDirectoryTreeRecursive(const std::string& path, std::vector<std::string>& result, int currentDepth, int maxDepth);
    };

}
