#include "FileManager.h"
#include <algorithm>
#include <fstream>

namespace FileSystemManager {

    FileManager::FileManager() : currentPath(fs::current_path().string()) {
        updateCache(currentPath);
    }

    FileManager::FileManager(const std::string& initialPath) : currentPath(initialPath) {
        if (!fs::exists(currentPath)) {
            currentPath = fs::current_path().string();
        }
        updateCache(currentPath);
    }

    bool FileManager::changeDirectory(const std::string& path) {
        try {
            fs::path newPath;
            if (fs::path(path).is_absolute()) {
                newPath = fs::path(path);
            } else {
                newPath = fs::path(currentPath) / path;
            }
            newPath = fs::canonical(newPath);
            
            if (fs::exists(newPath) && fs::is_directory(newPath)) {
                currentPath = newPath.string();
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    std::string FileManager::getCurrentPath() const {
        return currentPath;
    }

    std::string FileManager::getParentDirectory() const {
        try {
            return fs::path(currentPath).parent_path().string();
        } catch (const std::exception&) {
            return currentPath;
        }
    }

    void FileManager::updateCache(const std::string& path) {
        try {
            std::vector<FileInfo> files;
            for (const auto& entry : fs::directory_iterator(path)) {
                files.emplace_back(entry.path());
            }
            
            // Sort by name
            std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
                return a.name < b.name;
            });
            
            directoryCache[path] = std::move(files);
        } catch (const std::exception&) {
            directoryCache[path] = std::vector<FileInfo>();
        }
    }

    std::vector<FileInfo> FileManager::listFiles(bool includeHidden) {
        if (directoryCache.find(currentPath) == directoryCache.end()) {
            updateCache(currentPath);
        }
        
        std::vector<FileInfo> result = directoryCache[currentPath];
        
        if (!includeHidden) {
            result.erase(std::remove_if(result.begin(), result.end(),
                [](const FileInfo& info) {
                    return !info.name.empty() && info.name[0] == '.';
                }), result.end());
        }
        
        return result;
    }

    std::vector<FileInfo> FileManager::listFilesByExtension(const std::string& extension) {
        auto allFiles = listFiles();
        std::vector<FileInfo> result;
        
        std::string ext = extension;
        if (!ext.empty() && ext[0] != '.') {
            ext = "." + ext;
        }
        
        std::copy_if(allFiles.begin(), allFiles.end(), std::back_inserter(result),
            [&ext](const FileInfo& info) {
                return info.extension == ext;
            });
        
        return result;
    }

    std::vector<FileInfo> FileManager::listDirectories() {
        auto allFiles = listFiles();
        std::vector<FileInfo> result;
        
        std::copy_if(allFiles.begin(), allFiles.end(), std::back_inserter(result),
            [](const FileInfo& info) {
                return info.isDirectory;
            });
        
        return result;
    }

    bool FileManager::createFile(const std::string& fileName, const std::string& content) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            std::ofstream file(filePath);
            if (file.is_open()) {
                file << content;
                file.close();
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::createDirectory(const std::string& dirName) {
        try {
            fs::path dirPath = fs::path(currentPath) / dirName;
            if (fs::create_directory(dirPath)) {
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::deleteFile(const std::string& fileName) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            if (fs::exists(filePath) && !fs::is_directory(filePath)) {
                if (fs::remove(filePath)) {
                    updateCache(currentPath);
                    return true;
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::deleteDirectory(const std::string& dirName) {
        try {
            fs::path dirPath = fs::path(currentPath) / dirName;
            if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
                if (fs::remove_all(dirPath) > 0) {
                    updateCache(currentPath);
                    return true;
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::copyFile(const std::string& source, const std::string& destination) {
        try {
            fs::path sourcePath = fs::path(currentPath) / source;
            fs::path destPath = fs::path(currentPath) / destination;
            
            if (fs::exists(sourcePath) && !fs::is_directory(sourcePath)) {
                fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::moveFile(const std::string& source, const std::string& destination) {
        try {
            fs::path sourcePath = fs::path(currentPath) / source;
            fs::path destPath = fs::path(currentPath) / destination;
            
            if (fs::exists(sourcePath)) {
                fs::rename(sourcePath, destPath);
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::renameFile(const std::string& oldName, const std::string& newName) {
        return moveFile(oldName, newName);
    }

    FileInfo FileManager::getFileInfo(const std::string& fileName) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            return FileInfo(filePath);
        } catch (const std::exception&) {
            return FileInfo();
        }
    }

    bool FileManager::fileExists(const std::string& fileName) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            return fs::exists(filePath) && !fs::is_directory(filePath);
        } catch (const std::exception&) {
            return false;
        }
    }

    bool FileManager::directoryExists(const std::string& dirName) {
        try {
            fs::path dirPath = fs::path(currentPath) / dirName;
            return fs::exists(dirPath) && fs::is_directory(dirPath);
        } catch (const std::exception&) {
            return false;
        }
    }

    size_t FileManager::getDirectorySize(const std::string& dirPath) {
        try {
            size_t totalSize = 0;
            for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
                if (fs::is_regular_file(entry)) {
                    totalSize += fs::file_size(entry);
                }
            }
            return totalSize;
        } catch (const std::exception&) {
            return 0;
        }
    }

    std::string FileManager::readFileContent(const std::string& fileName) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            if (fs::exists(filePath) && !fs::is_directory(filePath)) {
                std::ifstream file(filePath);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
                    file.close();
                    return content;
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return "";
    }

    bool FileManager::writeFileContent(const std::string& fileName, const std::string& content) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            std::ofstream file(filePath);
            if (file.is_open()) {
                file << content;
                file.close();
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    bool FileManager::appendToFile(const std::string& fileName, const std::string& content) {
        try {
            fs::path filePath = fs::path(currentPath) / fileName;
            std::ofstream file(filePath, std::ios::app);
            if (file.is_open()) {
                file << content;
                file.close();
                updateCache(currentPath);
                return true;
            }
        } catch (const std::exception&) {
            // Error handling
        }
        return false;
    }

    void FileManager::refreshCache() {
        updateCache(currentPath);
    }

    void FileManager::clearCache() {
        directoryCache.clear();
    }

    std::vector<std::string> FileManager::getDirectoryTree(const std::string& path, int maxDepth) {
        std::vector<std::string> result;
        try {
            std::string searchPath = path.empty() ? currentPath : path;
            getDirectoryTreeRecursive(searchPath, result, 0, maxDepth);
        } catch (const std::exception&) {
            // Error handling
        }
        return result;
    }

    void FileManager::getDirectoryTreeRecursive(const std::string& path, std::vector<std::string>& result, int currentDepth, int maxDepth) {
        if (currentDepth >= maxDepth) return;
        
        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                std::string indent(currentDepth * 2, ' ');
                std::string entryName = entry.path().filename().string();
                
                if (fs::is_directory(entry)) {
                    result.push_back(indent + "📁 " + entryName + "/");
                    getDirectoryTreeRecursive(entry.path().string(), result, currentDepth + 1, maxDepth);
                } else {
                    result.push_back(indent + "📄 " + entryName);
                }
            }
        } catch (const std::exception&) {
            // Error handling
        }
    }

    void FileManager::addWatchedDirectory(const std::string& path) {
        watchedDirectories.insert(path);
    }

    void FileManager::removeWatchedDirectory(const std::string& path) {
        watchedDirectories.erase(path);
    }

    std::vector<std::string> FileManager::getChangedFiles() {
        std::vector<std::string> changedFiles;
        // Basic implementation - in a real application, you'd use file system watchers
        for (const auto& dir : watchedDirectories) {
            updateCache(dir);
        }
        return changedFiles;
    }

    bool FileManager::isTextFile(const std::string& filePath) {
        try {
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) return false;
            
            char buffer[1024];
            file.read(buffer, sizeof(buffer));
            size_t bytesRead = file.gcount();
            
            for (size_t i = 0; i < bytesRead; ++i) {
                if (buffer[i] == 0) {
                    return false; // Binary file
                }
            }
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }

}
