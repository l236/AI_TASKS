#include "BatchOperations.h"
#include <algorithm>
#include <iomanip>

namespace FileSystemManager {

    BatchOperations::BatchOperations() : operationInProgress(false), processedFiles(0), totalFiles(0) {
    }

    void BatchOperations::setProgressCallback(std::function<void(size_t, size_t, const std::string&)> callback) {
        progressCallback.callback = callback;
    }

    bool BatchOperations::isOperationInProgress() const {
        return operationInProgress.load();
    }

    double BatchOperations::getProgressPercentage() const {
        if (totalFiles.load() == 0) return 0.0;
        return static_cast<double>(processedFiles.load()) / totalFiles.load() * 100.0;
    }

    size_t BatchOperations::getProcessedFiles() const {
        return processedFiles.load();
    }

    size_t BatchOperations::getTotalFiles() const {
        return totalFiles.load();
    }

    OperationResult BatchOperations::copyFiles(const std::vector<std::string>& sourceFiles, const std::string& destinationDir) {
        operationInProgress = true;
        processedFiles = 0;
        totalFiles = sourceFiles.size();
        
        OperationResult result;
        result.success = true;
        result.filesProcessed = 0;
        result.filesSkipped = 0;
        
        try {
            fs::create_directories(destinationDir);
            
            for (const auto& sourceFile : sourceFiles) {
                if (!shouldContinue()) {
                    result.success = false;
                    result.message = "Operation cancelled";
                    break;
                }
                
                try {
                    fs::path sourcePath(sourceFile);
                    fs::path destPath = fs::path(destinationDir) / sourcePath.filename();
                    
                    if (fs::exists(sourcePath) && fs::is_regular_file(sourcePath)) {
                        if (fs::exists(destPath)) {
                            destPath = generateUniqueFileName(destinationDir, sourcePath.filename().string());
                        }
                        
                        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);
                        result.filesProcessed++;
                    } else {
                        result.filesSkipped++;
                        result.errors.push_back("File not found or not a regular file: " + sourceFile);
                    }
                } catch (const std::exception& e) {
                    result.filesSkipped++;
                    result.errors.push_back("Error copying " + sourceFile + ": " + e.what());
                }
                
                processedFiles++;
                updateProgress(processedFiles, sourceFile);
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error creating destination directory: " + std::string(e.what());
        }
        
        operationInProgress = false;
        return result;
    }

    OperationResult BatchOperations::copyDirectory(const std::string& sourceDir, const std::string& destinationDir, bool recursive) {
        operationInProgress = true;
        processedFiles = 0;
        totalFiles = 0;
        
        OperationResult result;
        result.success = true;
        result.filesProcessed = 0;
        result.filesSkipped = 0;
        
        try {
            // Count total files first
            if (recursive) {
                for (const auto& entry : fs::recursive_directory_iterator(sourceDir)) {
                    if (fs::is_regular_file(entry)) {
                        totalFiles++;
                    }
                }
            } else {
                for (const auto& entry : fs::directory_iterator(sourceDir)) {
                    if (fs::is_regular_file(entry)) {
                        totalFiles++;
                    }
                }
            }
            
            // Copy files
            if (recursive) {
                copyDirectoryRecursive(sourceDir, destinationDir, result);
            } else {
                copyDirectoryNonRecursive(sourceDir, destinationDir, result);
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error in directory copy operation: " + std::string(e.what());
        }
        
        operationInProgress = false;
        return result;
    }

    void BatchOperations::copyDirectoryRecursive(const std::string& sourceDir, const std::string& destinationDir, OperationResult& result) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(sourceDir)) {
                if (!shouldContinue()) {
                    result.success = false;
                    result.message = "Operation cancelled";
                    break;
                }
                
                if (fs::is_regular_file(entry)) {
                    try {
                        fs::path sourcePath = entry.path();
                        fs::path relativePath = fs::relative(sourcePath, sourceDir);
                        fs::path destPath = fs::path(destinationDir) / relativePath;
                        
                        fs::create_directories(destPath.parent_path());
                        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);
                        result.filesProcessed++;
                    } catch (const std::exception& e) {
                        result.filesSkipped++;
                        result.errors.push_back("Error copying " + entry.path().string() + ": " + e.what());
                    }
                }
                
                processedFiles++;
                updateProgress(processedFiles, entry.path().string());
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error in recursive directory copy: " + std::string(e.what());
        }
    }

    void BatchOperations::copyDirectoryNonRecursive(const std::string& sourceDir, const std::string& destinationDir, OperationResult& result) {
        try {
            for (const auto& entry : fs::directory_iterator(sourceDir)) {
                if (!shouldContinue()) {
                    result.success = false;
                    result.message = "Operation cancelled";
                    break;
                }
                
                if (fs::is_regular_file(entry)) {
                    try {
                        fs::path sourcePath = entry.path();
                        fs::path destPath = fs::path(destinationDir) / sourcePath.filename();
                        
                        fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);
                        result.filesProcessed++;
                    } catch (const std::exception& e) {
                        result.filesSkipped++;
                        result.errors.push_back("Error copying " + entry.path().string() + ": " + e.what());
                    }
                }
                
                processedFiles++;
                updateProgress(processedFiles, entry.path().string());
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error in directory copy: " + std::string(e.what());
        }
    }

    OperationResult BatchOperations::copyFilesByPattern(const std::string& sourceDir, const std::string& pattern, const std::string& destinationDir) {
        std::vector<std::string> matchingFiles;
        
        try {
            for (const auto& entry : fs::directory_iterator(sourceDir)) {
                if (fs::is_regular_file(entry)) {
                    std::string fileName = entry.path().filename().string();
                    if (matchesPattern(fileName, pattern)) {
                        matchingFiles.push_back(entry.path().string());
                    }
                }
            }
        } catch (const std::exception& e) {
            OperationResult result;
            result.success = false;
            result.message = "Error finding files: " + std::string(e.what());
            return result;
        }
        
        return copyFiles(matchingFiles, destinationDir);
    }

    OperationResult BatchOperations::moveFiles(const std::vector<std::string>& sourceFiles, const std::string& destinationDir) {
        operationInProgress = true;
        processedFiles = 0;
        totalFiles = sourceFiles.size();
        
        OperationResult result;
        result.success = true;
        result.filesProcessed = 0;
        result.filesSkipped = 0;
        
        try {
            fs::create_directories(destinationDir);
            
            for (const auto& sourceFile : sourceFiles) {
                if (!shouldContinue()) {
                    result.success = false;
                    result.message = "Operation cancelled";
                    break;
                }
                
                try {
                    fs::path sourcePath(sourceFile);
                    fs::path destPath = fs::path(destinationDir) / sourcePath.filename();
                    
                    if (fs::exists(sourcePath) && fs::is_regular_file(sourcePath)) {
                        if (fs::exists(destPath)) {
                            destPath = generateUniqueFileName(destinationDir, sourcePath.filename().string());
                        }
                        
                        fs::rename(sourcePath, destPath);
                        result.filesProcessed++;
                    } else {
                        result.filesSkipped++;
                        result.errors.push_back("File not found or not a regular file: " + sourceFile);
                    }
                } catch (const std::exception& e) {
                    result.filesSkipped++;
                    result.errors.push_back("Error moving " + sourceFile + ": " + e.what());
                }
                
                processedFiles++;
                updateProgress(processedFiles, sourceFile);
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error creating destination directory: " + std::string(e.what());
        }
        
        operationInProgress = false;
        return result;
    }

    OperationResult BatchOperations::moveFilesByPattern(const std::string& sourceDir, const std::string& pattern, const std::string& destinationDir) {
        std::vector<std::string> matchingFiles;
        
        try {
            for (const auto& entry : fs::directory_iterator(sourceDir)) {
                if (fs::is_regular_file(entry)) {
                    std::string fileName = entry.path().filename().string();
                    if (matchesPattern(fileName, pattern)) {
                        matchingFiles.push_back(entry.path().string());
                    }
                }
            }
        } catch (const std::exception& e) {
            OperationResult result;
            result.success = false;
            result.message = "Error finding files: " + std::string(e.what());
            return result;
        }
        
        return moveFiles(matchingFiles, destinationDir);
    }

    OperationResult BatchOperations::deleteFiles(const std::vector<std::string>& files) {
        operationInProgress = true;
        processedFiles = 0;
        totalFiles = files.size();
        
        OperationResult result;
        result.success = true;
        result.filesProcessed = 0;
        result.filesSkipped = 0;
        
        for (const auto& file : files) {
            if (!shouldContinue()) {
                result.success = false;
                result.message = "Operation cancelled";
                break;
            }
            
            try {
                fs::path filePath(file);
                if (fs::exists(filePath)) {
                    if (fs::is_regular_file(filePath)) {
                        fs::remove(filePath);
                        result.filesProcessed++;
                    } else if (fs::is_directory(filePath)) {
                        fs::remove_all(filePath);
                        result.filesProcessed++;
                    } else {
                        result.filesSkipped++;
                        result.errors.push_back("Unknown file type: " + file);
                    }
                } else {
                    result.filesSkipped++;
                    result.errors.push_back("File not found: " + file);
                }
            } catch (const std::exception& e) {
                result.filesSkipped++;
                result.errors.push_back("Error deleting " + file + ": " + e.what());
            }
            
            processedFiles++;
            updateProgress(processedFiles, file);
        }
        
        operationInProgress = false;
        return result;
    }

    OperationResult BatchOperations::deleteFilesByPattern(const std::string& directory, const std::string& pattern) {
        std::vector<std::string> matchingFiles;
        
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (fs::is_regular_file(entry)) {
                    std::string fileName = entry.path().filename().string();
                    if (matchesPattern(fileName, pattern)) {
                        matchingFiles.push_back(entry.path().string());
                    }
                }
            }
        } catch (const std::exception& e) {
            OperationResult result;
            result.success = false;
            result.message = "Error finding files: " + std::string(e.what());
            return result;
        }
        
        return deleteFiles(matchingFiles);
    }

    OperationResult BatchOperations::deleteEmptyDirectories(const std::string& directory, bool recursive) {
        operationInProgress = true;
        processedFiles = 0;
        totalFiles = 0;
        
        OperationResult result;
        result.success = true;
        result.filesProcessed = 0;
        result.filesSkipped = 0;
        
        try {
            if (recursive) {
                deleteEmptyDirectoriesRecursive(directory, result);
            } else {
                deleteEmptyDirectoriesNonRecursive(directory, result);
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error in empty directory deletion: " + std::string(e.what());
        }
        
        operationInProgress = false;
        return result;
    }

    void BatchOperations::deleteEmptyDirectoriesRecursive(const std::string& directory, OperationResult& result) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (!shouldContinue()) {
                    result.success = false;
                    result.message = "Operation cancelled";
                    break;
                }
                
                if (fs::is_directory(entry)) {
                    try {
                        if (fs::is_empty(entry)) {
                            fs::remove(entry);
                            result.filesProcessed++;
                        }
                    } catch (const std::exception& e) {
                        result.filesSkipped++;
                        result.errors.push_back("Error deleting empty directory " + entry.path().string() + ": " + e.what());
                    }
                }
                
                processedFiles++;
                updateProgress(processedFiles, entry.path().string());
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error in recursive empty directory deletion: " + std::string(e.what());
        }
    }

    void BatchOperations::deleteEmptyDirectoriesNonRecursive(const std::string& directory, OperationResult& result) {
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (!shouldContinue()) {
                    result.success = false;
                    result.message = "Operation cancelled";
                    break;
                }
                
                if (fs::is_directory(entry)) {
                    try {
                        if (fs::is_empty(entry)) {
                            fs::remove(entry);
                            result.filesProcessed++;
                        }
                    } catch (const std::exception& e) {
                        result.filesSkipped++;
                        result.errors.push_back("Error deleting empty directory " + entry.path().string() + ": " + e.what());
                    }
                }
                
                processedFiles++;
                updateProgress(processedFiles, entry.path().string());
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Error in empty directory deletion: " + std::string(e.what());
        }
    }

    void BatchOperations::updateProgress(size_t current, const std::string& currentFile) {
        if (progressCallback.callback) {
            progressCallback.callback(current, totalFiles.load(), currentFile);
        }
    }

    bool BatchOperations::shouldContinue() const {
        return !operationInProgress.load() || processedFiles.load() < totalFiles.load();
    }

    std::string BatchOperations::generateUniqueFileName(const std::string& directory, const std::string& fileName) {
        fs::path basePath = fs::path(directory) / fileName;
        fs::path result = basePath;
        
        int counter = 1;
        while (fs::exists(result)) {
            std::string stem = basePath.stem().string();
            std::string extension = basePath.extension().string();
            result = basePath.parent_path() / (stem + "_" + std::to_string(counter) + extension);
            counter++;
        }
        
        return result.string();
    }

    void BatchOperations::cancelOperation() {
        operationInProgress = false;
    }

    void BatchOperations::resetProgress() {
        processedFiles = 0;
        totalFiles = 0;
        operationInProgress = false;
    }

    std::future<OperationResult> BatchOperations::copyFilesAsync(const std::vector<std::string>& sourceFiles, const std::string& destinationDir) {
        return std::async(std::launch::async, [this, sourceFiles, destinationDir]() {
            return copyFiles(sourceFiles, destinationDir);
        });
    }

    std::future<OperationResult> BatchOperations::deleteFilesAsync(const std::vector<std::string>& files) {
        return std::async(std::launch::async, [this, files]() {
            return deleteFiles(files);
        });
    }

}

