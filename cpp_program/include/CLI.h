#pragma once

#include "Common.h"
#include "FileManager.h"
#include "SearchEngine.h"
#include "BatchOperations.h"
#include <map>
#include <functional>

namespace FileSystemManager {

    class CLI {
    private:
        FileManager fileManager;
        SearchEngine searchEngine;
        BatchOperations batchOps;
        
        std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands;
        bool running;
        std::string prompt;
        
        void initializeCommands();
        void printHelp();
        void printVersion();
        void printBanner();
        
        // Command handlers
        void handleHelp(const std::vector<std::string>& args);
        void handleVersion(const std::vector<std::string>& args);
        void handleExit(const std::vector<std::string>& args);
        void handlePwd(const std::vector<std::string>& args);
        void handleCd(const std::vector<std::string>& args);
        void handleLs(const std::vector<std::string>& args);
        void handleMkdir(const std::vector<std::string>& args);
        void handleTouch(const std::vector<std::string>& args);
        void handleRm(const std::vector<std::string>& args);
        void handleCp(const std::vector<std::string>& args);
        void handleMv(const std::vector<std::string>& args);
        void handleCat(const std::vector<std::string>& args);
        void handleEcho(const std::vector<std::string>& args);
        void handleFind(const std::vector<std::string>& args);
        void handleGrep(const std::vector<std::string>& args);
        void handleSize(const std::vector<std::string>& args);
        void handleTree(const std::vector<std::string>& args);
        void handleSearch(const std::vector<std::string>& args);
        void handleBatch(const std::vector<std::string>& args);
        void handleStats(const std::vector<std::string>& args);
        void handleClear(const std::vector<std::string>& args);
        
        // Utility functions
        std::vector<std::string> parseCommand(const std::string& input);
        void printError(const std::string& message);
        void printSuccess(const std::string& message);
        void printInfo(const std::string& message);
        void printFileInfo(const FileInfo& info);
        void printSearchResult(const SearchResult& result);
        void printOperationResult(const OperationResult& result);
        
        // Formatting
        std::string formatFileSize(size_t bytes);
        std::string formatTimestamp(const std::string& timestamp);
        void printTable(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers);
        
    public:
        CLI();
        explicit CLI(const std::string& initialPath);
        ~CLI() = default;
        
        // Main interface
        void run();
        void runScript(const std::string& scriptPath);
        void executeCommand(const std::string& command);
        
        // Configuration
        void setPrompt(const std::string& newPrompt);
        void setVerbose(bool verbose);
        
        // Interactive features
        void enableAutoComplete();
        void enableHistory();
        void saveHistory(const std::string& filePath);
        void loadHistory(const std::string& filePath);
        
        // Command registration
        void registerCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> handler);
        void unregisterCommand(const std::string& name);
        
        // Status
        bool isRunning() const;
        std::string getCurrentPath() const;
        
    private:
        bool verbose;
        std::vector<std::string> commandHistory;
        std::string historyFilePath;
    };

}
