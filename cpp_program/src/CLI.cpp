#include "CLI.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace FileSystemManager {

    CLI::CLI() : fileManager(), searchEngine(), batchOps(), running(false), prompt("fsmanager> "), verbose(false) {
        initializeCommands();
    }

    CLI::CLI(const std::string& initialPath) : fileManager(initialPath), searchEngine(initialPath), batchOps(), running(false), prompt("fsmanager> "), verbose(false) {
        initializeCommands();
    }

    void CLI::initializeCommands() {
        commands["help"] = [this](const std::vector<std::string>& args) { handleHelp(args); };
        commands["h"] = [this](const std::vector<std::string>& args) { handleHelp(args); };
        commands["version"] = [this](const std::vector<std::string>& args) { handleVersion(args); };
        commands["v"] = [this](const std::vector<std::string>& args) { handleVersion(args); };
        commands["exit"] = [this](const std::vector<std::string>& args) { handleExit(args); };
        commands["quit"] = [this](const std::vector<std::string>& args) { handleExit(args); };
        commands["q"] = [this](const std::vector<std::string>& args) { handleExit(args); };
        commands["pwd"] = [this](const std::vector<std::string>& args) { handlePwd(args); };
        commands["cd"] = [this](const std::vector<std::string>& args) { handleCd(args); };
        commands["ls"] = [this](const std::vector<std::string>& args) { handleLs(args); };
        commands["dir"] = [this](const std::vector<std::string>& args) { handleLs(args); };
        commands["mkdir"] = [this](const std::vector<std::string>& args) { handleMkdir(args); };
        commands["touch"] = [this](const std::vector<std::string>& args) { handleTouch(args); };
        commands["rm"] = [this](const std::vector<std::string>& args) { handleRm(args); };
        commands["del"] = [this](const std::vector<std::string>& args) { handleRm(args); };
        commands["cp"] = [this](const std::vector<std::string>& args) { handleCp(args); };
        commands["copy"] = [this](const std::vector<std::string>& args) { handleCp(args); };
        commands["mv"] = [this](const std::vector<std::string>& args) { handleMv(args); };
        commands["move"] = [this](const std::vector<std::string>& args) { handleMv(args); };
        commands["cat"] = [this](const std::vector<std::string>& args) { handleCat(args); };
        commands["echo"] = [this](const std::vector<std::string>& args) { handleEcho(args); };
        commands["find"] = [this](const std::vector<std::string>& args) { handleFind(args); };
        commands["grep"] = [this](const std::vector<std::string>& args) { handleGrep(args); };
        commands["size"] = [this](const std::vector<std::string>& args) { handleSize(args); };
        commands["tree"] = [this](const std::vector<std::string>& args) { handleTree(args); };
        commands["search"] = [this](const std::vector<std::string>& args) { handleSearch(args); };
        commands["batch"] = [this](const std::vector<std::string>& args) { handleBatch(args); };
        commands["stats"] = [this](const std::vector<std::string>& args) { handleStats(args); };
        commands["clear"] = [this](const std::vector<std::string>& args) { handleClear(args); };
        commands["cls"] = [this](const std::vector<std::string>& args) { handleClear(args); };
    }

    void CLI::run() {
        printBanner();
        running = true;
        
        std::string input;
        while (running) {
            std::cout << prompt;
            std::getline(std::cin, input);
            
            if (!input.empty()) {
                commandHistory.push_back(input);
                executeCommand(input);
            }
        }
    }

    void CLI::runScript(const std::string& scriptPath) {
        std::ifstream script(scriptPath);
        if (!script.is_open()) {
            printError("Cannot open script file: " + scriptPath);
            return;
        }
        
        std::string line;
        while (std::getline(script, line) && running) {
            if (!line.empty() && line[0] != '#') { // Skip empty lines and comments
                std::cout << prompt << line << std::endl;
                executeCommand(line);
            }
        }
        script.close();
    }

    void CLI::executeCommand(const std::string& command) {
        auto args = parseCommand(command);
        if (args.empty()) return;
        
        std::string cmd = args[0];
        args.erase(args.begin());
        
        auto it = commands.find(cmd);
        if (it != commands.end()) {
            try {
                it->second(args);
            } catch (const std::exception& e) {
                printError("Error executing command: " + std::string(e.what()));
            }
        } else {
            printError("Unknown command: " + cmd + ". Type 'help' for available commands.");
        }
    }

    std::vector<std::string> CLI::parseCommand(const std::string& input) {
        std::vector<std::string> args;
        std::stringstream ss(input);
        std::string arg;
        
        while (ss >> arg) {
            args.push_back(arg);
        }
        
        return args;
    }

    void CLI::printBanner() {
        std::cout << "===============================================" << std::endl;
        std::cout << "    File System Manager v1.0.0" << std::endl;
        std::cout << "    Advanced file operations and management" << std::endl;
        std::cout << "    Type 'help' for available commands" << std::endl;
        std::cout << "===============================================" << std::endl;
        std::cout << std::endl;
    }

    void CLI::printHelp() {
        std::cout << "Available commands:" << std::endl;
        std::cout << "  Navigation:" << std::endl;
        std::cout << "    pwd                    - Print current directory" << std::endl;
        std::cout << "    cd <path>              - Change directory" << std::endl;
        std::cout << "    ls [options]           - List files and directories" << std::endl;
        std::cout << "    tree [depth]           - Show directory tree" << std::endl;
        std::cout << std::endl;
        std::cout << "  File Operations:" << std::endl;
        std::cout << "    touch <file>           - Create empty file" << std::endl;
        std::cout << "    mkdir <dir>            - Create directory" << std::endl;
        std::cout << "    rm <file>              - Delete file" << std::endl;
        std::cout << "    cp <src> <dest>        - Copy file" << std::endl;
        std::cout << "    mv <src> <dest>        - Move/rename file" << std::endl;
        std::cout << "    cat <file>             - Display file content" << std::endl;
        std::cout << "    echo <text> > <file>   - Write text to file" << std::endl;
        std::cout << std::endl;
        std::cout << "  Search Operations:" << std::endl;
        std::cout << "    find <pattern>         - Find files by name pattern" << std::endl;
        std::cout << "    grep <term> [file]     - Search content in files" << std::endl;
        std::cout << "    search <options>       - Advanced search" << std::endl;
        std::cout << std::endl;
        std::cout << "  Batch Operations:" << std::endl;
        std::cout << "    batch copy <pattern> <dest>  - Copy files by pattern" << std::endl;
        std::cout << "    batch move <pattern> <dest>  - Move files by pattern" << std::endl;
        std::cout << "    batch delete <pattern>       - Delete files by pattern" << std::endl;
        std::cout << std::endl;
        std::cout << "  Utilities:" << std::endl;
        std::cout << "    size <path>            - Show file/directory size" << std::endl;
        std::cout << "    stats                  - Show current directory statistics" << std::endl;
        std::cout << "    clear                  - Clear screen" << std::endl;
        std::cout << "    help                   - Show this help" << std::endl;
        std::cout << "    exit                   - Exit program" << std::endl;
        std::cout << std::endl;
    }

    void CLI::handleHelp(const std::vector<std::string>& args) {
        printHelp();
    }

    void CLI::handleVersion(const std::vector<std::string>& args) {
        std::cout << "File System Manager v1.0.0" << std::endl;
        std::cout << "Built with C++17 and std::filesystem" << std::endl;
    }

    void CLI::handleExit(const std::vector<std::string>& args) {
        running = false;
        std::cout << "Goodbye!" << std::endl;
    }

    void CLI::handlePwd(const std::vector<std::string>& args) {
        std::cout << fileManager.getCurrentPath() << std::endl;
    }

    void CLI::handleCd(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: cd <path>");
            return;
        }
        
        std::string path = args[0];
        if (path == "..") {
            path = fileManager.getParentDirectory();
        }
        
        if (fileManager.changeDirectory(path)) {
            searchEngine.setSearchRoot(fileManager.getCurrentPath());
            printSuccess("Changed to: " + fileManager.getCurrentPath());
        } else {
            printError("Cannot change to directory: " + path);
        }
    }

    void CLI::handleLs(const std::vector<std::string>& args) {
        bool showHidden = false;
        bool showDetails = false;
        
        for (const auto& arg : args) {
            if (arg == "-a" || arg == "--all") {
                showHidden = true;
            } else if (arg == "-l" || arg == "--long") {
                showDetails = true;
            }
        }
        
        auto files = fileManager.listFiles(showHidden);
        
        if (showDetails) {
            std::cout << std::left << std::setw(20) << "Name" 
                      << std::setw(12) << "Size" 
                      << std::setw(20) << "Modified" 
                      << std::setw(8) << "Type" << std::endl;
            std::cout << std::string(60, '-') << std::endl;
            
            for (const auto& file : files) {
                std::cout << std::left << std::setw(20) << file.name
                          << std::setw(12) << formatFileSize(file.size)
                          << std::setw(20) << file.lastModified
                          << std::setw(8) << (file.isDirectory ? "DIR" : "FILE") << std::endl;
            }
        } else {
            for (const auto& file : files) {
                std::cout << (file.isDirectory ? "📁 " : "📄 ") << file.name << std::endl;
            }
        }
    }

    void CLI::handleMkdir(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: mkdir <directory_name>");
            return;
        }
        
        for (const auto& dirName : args) {
            if (fileManager.createDirectory(dirName)) {
                printSuccess("Created directory: " + dirName);
            } else {
                printError("Failed to create directory: " + dirName);
            }
        }
    }

    void CLI::handleTouch(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: touch <file_name>");
            return;
        }
        
        for (const auto& fileName : args) {
            if (fileManager.createFile(fileName)) {
                printSuccess("Created file: " + fileName);
            } else {
                printError("Failed to create file: " + fileName);
            }
        }
    }

    void CLI::handleRm(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: rm <file_name>");
            return;
        }
        
        for (const auto& fileName : args) {
            if (fileManager.deleteFile(fileName)) {
                printSuccess("Deleted file: " + fileName);
            } else {
                printError("Failed to delete file: " + fileName);
            }
        }
    }

    void CLI::handleCp(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            printError("Usage: cp <source> <destination>");
            return;
        }
        
        std::string source = args[0];
        std::string destination = args[1];
        
        if (fileManager.copyFile(source, destination)) {
            printSuccess("Copied " + source + " to " + destination);
        } else {
            printError("Failed to copy " + source + " to " + destination);
        }
    }

    void CLI::handleMv(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            printError("Usage: mv <source> <destination>");
            return;
        }
        
        std::string source = args[0];
        std::string destination = args[1];
        
        if (fileManager.moveFile(source, destination)) {
            printSuccess("Moved " + source + " to " + destination);
        } else {
            printError("Failed to move " + source + " to " + destination);
        }
    }

    void CLI::handleCat(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: cat <file_name>");
            return;
        }
        
        for (const auto& fileName : args) {
            std::string content = fileManager.readFileContent(fileName);
            if (!content.empty()) {
                std::cout << "=== " << fileName << " ===" << std::endl;
                std::cout << content << std::endl;
            } else {
                printError("Cannot read file: " + fileName);
            }
        }
    }

    void CLI::handleEcho(const std::vector<std::string>& args) {
        if (args.size() < 3 || args[1] != ">") {
            printError("Usage: echo <text> > <file_name>");
            return;
        }
        
        std::string text = args[0];
        std::string fileName = args[2];
        
        if (fileManager.writeFileContent(fileName, text)) {
            printSuccess("Wrote to file: " + fileName);
        } else {
            printError("Failed to write to file: " + fileName);
        }
    }

    void CLI::handleFind(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: find <pattern>");
            return;
        }
        
        std::string pattern = args[0];
        auto results = searchEngine.searchByName(pattern, true);
        
        if (results.empty()) {
            printInfo("No files found matching pattern: " + pattern);
        } else {
            std::cout << "Found " << results.size() << " files:" << std::endl;
            for (const auto& result : results) {
                std::cout << "  " << result.filePath << " (" << formatFileSize(result.fileSize) << ")" << std::endl;
            }
        }
    }

    void CLI::handleGrep(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: grep <search_term> [file_pattern]");
            return;
        }
        
        std::string searchTerm = args[0];
        std::string filePattern = args.size() > 1 ? args[1] : "*";
        
        auto results = searchEngine.searchInContent(searchTerm, true);
        
        if (results.empty()) {
            printInfo("No files found containing: " + searchTerm);
        } else {
            std::cout << "Found " << results.size() << " files containing '" << searchTerm << "':" << std::endl;
            for (const auto& result : results) {
                std::cout << "  " << result.filePath << std::endl;
                for (const auto& line : result.matchingLines) {
                    std::cout << "    " << line << std::endl;
                }
            }
        }
    }

    void CLI::handleSize(const std::vector<std::string>& args) {
        if (args.empty()) {
            // Show current directory size
            size_t size = fileManager.getDirectorySize(fileManager.getCurrentPath());
            std::cout << "Directory size: " << formatFileSize(size) << std::endl;
        } else {
            for (const auto& path : args) {
                if (fileManager.fileExists(path)) {
                    auto info = fileManager.getFileInfo(path);
                    std::cout << path << ": " << formatFileSize(info.size) << std::endl;
                } else if (fileManager.directoryExists(path)) {
                    size_t size = fileManager.getDirectorySize(path);
                    std::cout << path << ": " << formatFileSize(size) << std::endl;
                } else {
                    printError("File or directory not found: " + path);
                }
            }
        }
    }

    void CLI::handleTree(const std::vector<std::string>& args) {
        int maxDepth = 3;
        if (!args.empty()) {
            try {
                maxDepth = std::stoi(args[0]);
            } catch (const std::exception&) {
                printError("Invalid depth value: " + args[0]);
                return;
            }
        }
        
        auto tree = fileManager.getDirectoryTree("", maxDepth);
        for (const auto& line : tree) {
            std::cout << line << std::endl;
        }
    }

    void CLI::handleSearch(const std::vector<std::string>& args) {
        if (args.empty()) {
            printError("Usage: search <options>");
            printInfo("Options: -name <pattern>, -content <term>, -size <min>-<max>, -ext <extension>");
            return;
        }
        
        // Simple search implementation
        std::string pattern = args[0];
        auto results = searchEngine.searchByName(pattern, true);
        
        std::cout << "Search results for '" << pattern << "':" << std::endl;
        for (const auto& result : results) {
            std::cout << "  " << result.filePath << " (" << formatFileSize(result.fileSize) << ")" << std::endl;
        }
    }

    void CLI::handleBatch(const std::vector<std::string>& args) {
        if (args.size() < 3) {
            printError("Usage: batch <operation> <pattern> <destination>");
            printInfo("Operations: copy, move, delete");
            return;
        }
        
        std::string operation = args[0];
        std::string pattern = args[1];
        std::string destination = args.size() > 2 ? args[2] : "";
        
        OperationResult result;
        
        if (operation == "copy") {
            result = batchOps.copyFilesByPattern(fileManager.getCurrentPath(), pattern, destination);
        } else if (operation == "move") {
            result = batchOps.moveFilesByPattern(fileManager.getCurrentPath(), pattern, destination);
        } else if (operation == "delete") {
            result = batchOps.deleteFilesByPattern(fileManager.getCurrentPath(), pattern);
        } else {
            printError("Unknown batch operation: " + operation);
            return;
        }
        
        printOperationResult(result);
    }

    void CLI::handleStats(const std::vector<std::string>& args) {
        auto files = fileManager.listFiles();
        size_t totalFiles = 0;
        size_t totalDirs = 0;
        size_t totalSize = 0;
        
        for (const auto& file : files) {
            if (file.isDirectory) {
                totalDirs++;
            } else {
                totalFiles++;
                totalSize += file.size;
            }
        }
        
        std::cout << "Directory Statistics:" << std::endl;
        std::cout << "  Current path: " << fileManager.getCurrentPath() << std::endl;
        std::cout << "  Files: " << totalFiles << std::endl;
        std::cout << "  Directories: " << totalDirs << std::endl;
        std::cout << "  Total size: " << formatFileSize(totalSize) << std::endl;
    }

    void CLI::handleClear(const std::vector<std::string>& args) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void CLI::printError(const std::string& message) {
        std::cout << "❌ Error: " << message << std::endl;
    }

    void CLI::printSuccess(const std::string& message) {
        std::cout << "✅ " << message << std::endl;
    }

    void CLI::printInfo(const std::string& message) {
        std::cout << "ℹ️  " << message << std::endl;
    }

    void CLI::printFileInfo(const FileInfo& info) {
        std::cout << "Name: " << info.name << std::endl;
        std::cout << "Path: " << info.path << std::endl;
        std::cout << "Size: " << formatFileSize(info.size) << std::endl;
        std::cout << "Modified: " << info.lastModified << std::endl;
        std::cout << "Type: " << (info.isDirectory ? "Directory" : "File") << std::endl;
    }

    void CLI::printSearchResult(const SearchResult& result) {
        std::cout << "File: " << result.filePath << std::endl;
        std::cout << "Size: " << formatFileSize(result.fileSize) << std::endl;
        std::cout << "Modified: " << result.lastModified << std::endl;
        if (!result.matchingLines.empty()) {
            std::cout << "Matching lines:" << std::endl;
            for (const auto& line : result.matchingLines) {
                std::cout << "  " << line << std::endl;
            }
        }
    }

    void CLI::printOperationResult(const OperationResult& result) {
        if (result.success) {
            printSuccess("Operation completed successfully");
        } else {
            printError("Operation failed: " + result.message);
        }
        
        std::cout << "Files processed: " << result.filesProcessed << std::endl;
        std::cout << "Files skipped: " << result.filesSkipped << std::endl;
        
        if (!result.errors.empty()) {
            std::cout << "Errors:" << std::endl;
            for (const auto& error : result.errors) {
                std::cout << "  " << error << std::endl;
            }
        }
    }

    std::string CLI::formatFileSize(size_t bytes) {
        return FileSystemManager::formatFileSize(bytes);
    }

    std::string CLI::formatTimestamp(const std::string& timestamp) {
        return timestamp; // Already formatted in Common.cpp
    }

    void CLI::printTable(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers) {
        // Simple table printing implementation
        for (const auto& header : headers) {
            std::cout << std::setw(20) << header;
        }
        std::cout << std::endl;
        
        for (const auto& row : data) {
            for (const auto& cell : row) {
                std::cout << std::setw(20) << cell;
            }
            std::cout << std::endl;
        }
    }

    void CLI::setPrompt(const std::string& newPrompt) {
        prompt = newPrompt;
    }

    void CLI::setVerbose(bool verbose) {
        this->verbose = verbose;
    }

    void CLI::enableAutoComplete() {
        // Placeholder for auto-complete functionality
    }

    void CLI::enableHistory() {
        // History is already enabled by default
    }

    void CLI::saveHistory(const std::string& filePath) {
        std::ofstream file(filePath);
        if (file.is_open()) {
            for (const auto& command : commandHistory) {
                file << command << std::endl;
            }
            file.close();
        }
    }

    void CLI::loadHistory(const std::string& filePath) {
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                commandHistory.push_back(line);
            }
            file.close();
        }
    }

    void CLI::registerCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> handler) {
        commands[name] = handler;
    }

    void CLI::unregisterCommand(const std::string& name) {
        commands.erase(name);
    }

    bool CLI::isRunning() const {
        return running;
    }

    std::string CLI::getCurrentPath() const {
        return fileManager.getCurrentPath();
    }

}

