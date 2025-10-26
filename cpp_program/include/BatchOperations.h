#pragma once

#include "Common.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <functional>

namespace FileSystemManager {

    class BatchOperations {
    private:
        std::atomic<bool> operationInProgress;
        std::atomic<size_t> processedFiles;
        std::atomic<size_t> totalFiles;
        std::mutex progressMutex;
        
        struct ProgressCallback {
            std::function<void(size_t current, size_t total, const std::string& currentFile)> callback;
        };
        
        ProgressCallback progressCallback;
        
    public:
        BatchOperations();
        ~BatchOperations() = default;
        
        // Progress tracking
        void setProgressCallback(std::function<void(size_t, size_t, const std::string&)> callback);
        bool isOperationInProgress() const;
        double getProgressPercentage() const;
        size_t getProcessedFiles() const;
        size_t getTotalFiles() const;
        
        // Batch copy operations
        OperationResult copyFiles(const std::vector<std::string>& sourceFiles, const std::string& destinationDir);
        OperationResult copyDirectory(const std::string& sourceDir, const std::string& destinationDir, bool recursive = true);
        OperationResult copyFilesByPattern(const std::string& sourceDir, const std::string& pattern, const std::string& destinationDir);
        
        // Batch move operations
        OperationResult moveFiles(const std::vector<std::string>& sourceFiles, const std::string& destinationDir);
        OperationResult moveFilesByPattern(const std::string& sourceDir, const std::string& pattern, const std::string& destinationDir);
        
        // Batch delete operations
        OperationResult deleteFiles(const std::vector<std::string>& files);
        OperationResult deleteFilesByPattern(const std::string& directory, const std::string& pattern);
        OperationResult deleteEmptyDirectories(const std::string& directory, bool recursive = true);
        
        // Batch rename operations
        OperationResult renameFiles(const std::vector<std::string>& files, const std::string& newPattern);
        OperationResult renameFilesByPattern(const std::string& directory, const std::string& oldPattern, const std::string& newPattern);
        OperationResult addPrefix(const std::vector<std::string>& files, const std::string& prefix);
        OperationResult addSuffix(const std::vector<std::string>& files, const std::string& suffix);
        
        // File organization
        OperationResult organizeByExtension(const std::string& sourceDir, const std::string& destinationDir);
        OperationResult organizeByDate(const std::string& sourceDir, const std::string& destinationDir, const std::string& dateFormat = "%Y-%m-%d");
        OperationResult organizeBySize(const std::string& sourceDir, const std::string& destinationDir, 
                                     const std::vector<std::pair<size_t, std::string>>& sizeRanges);
        
        // File conversion
        OperationResult convertTextEncoding(const std::vector<std::string>& files, const std::string& fromEncoding, const std::string& toEncoding);
        OperationResult normalizeLineEndings(const std::vector<std::string>& files, const std::string& targetLineEnding = "LF");
        
        // File cleanup
        OperationResult removeEmptyFiles(const std::string& directory, bool recursive = true);
        OperationResult removeDuplicateFiles(const std::string& directory, bool byContent = true);
        OperationResult cleanupTempFiles(const std::string& directory, const std::vector<std::string>& tempExtensions = {".tmp", ".temp", ".bak"});
        
        // Async operations
        std::future<OperationResult> copyFilesAsync(const std::vector<std::string>& sourceFiles, const std::string& destinationDir);
        std::future<OperationResult> deleteFilesAsync(const std::vector<std::string>& files);
        
        // Operation cancellation
        void cancelOperation();
        void resetProgress();
        
    private:
        void updateProgress(size_t current, const std::string& currentFile = "");
        bool shouldContinue() const;
        std::string generateUniqueFileName(const std::string& directory, const std::string& fileName);
        void copyDirectoryRecursive(const std::string& sourceDir, const std::string& destinationDir, OperationResult& result);
        void copyDirectoryNonRecursive(const std::string& sourceDir, const std::string& destinationDir, OperationResult& result);
        void deleteEmptyDirectoriesRecursive(const std::string& directory, OperationResult& result);
        void deleteEmptyDirectoriesNonRecursive(const std::string& directory, OperationResult& result);
    };

}
