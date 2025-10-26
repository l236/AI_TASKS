#include "CLI.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        FileSystemManager::CLI cli;
        
        // Check for command line arguments
        if (argc > 1) {
            std::string arg = argv[1];
            
            if (arg == "--help" || arg == "-h") {
                std::cout << "File System Manager v1.0.0" << std::endl;
                std::cout << "Usage: " << argv[0] << " [options] [initial_path]" << std::endl;
                std::cout << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "  --help, -h     Show this help message" << std::endl;
                std::cout << "  --version, -v  Show version information" << std::endl;
                std::cout << "  --script <file> Run commands from script file" << std::endl;
                std::cout << std::endl;
                std::cout << "Arguments:" << std::endl;
                std::cout << "  initial_path   Start in the specified directory" << std::endl;
                return 0;
            } else if (arg == "--version" || arg == "-v") {
                std::cout << "File System Manager v1.0.0" << std::endl;
                std::cout << "Built with C++17 and std::filesystem" << std::endl;
                return 0;
            } else if (arg == "--script" && argc > 2) {
                std::string scriptPath = argv[2];
                cli.runScript(scriptPath);
                return 0;
            } else {
                // Treat as initial path
                FileSystemManager::CLI cliWithPath(arg);
                cliWithPath.run();
            }
        } else {
            // No arguments, start interactive mode
            cli.run();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
    
    return 0;
}
