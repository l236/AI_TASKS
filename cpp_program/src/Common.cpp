#include "Common.h"
#include <algorithm>
#include <regex>

namespace FileSystemManager {

    FileInfo::FileInfo(const fs::path& filePath) {
        try {
            if (fs::exists(filePath)) {
                name = filePath.filename().string();
                path = filePath.string();
                extension = filePath.extension().string();
                size = fs::is_directory(filePath) ? 0 : fs::file_size(filePath);
                
                auto ftime = fs::last_write_time(filePath);
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                lastModified = formatTimestamp(sctp);
                
                isDirectory = fs::is_directory(filePath);
            }
        } catch (const std::exception& e) {
            // Handle errors gracefully
            name = filePath.filename().string();
            path = filePath.string();
            extension = "";
            size = 0;
            lastModified = "Unknown";
            isDirectory = false;
        }
    }

    std::string formatFileSize(size_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unit = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024.0 && unit < 4) {
            size /= 1024.0;
            unit++;
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << size << " " << units[unit];
        return oss.str();
    }

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        return formatTimestamp(now);
    }

    std::string formatTimestamp(const std::chrono::system_clock::time_point& time) {
        auto time_t = std::chrono::system_clock::to_time_t(time);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    bool isValidPath(const std::string& path) {
        try {
            fs::path p(path);
            return fs::exists(p) || fs::is_directory(p.parent_path());
        } catch (const std::exception&) {
            return false;
        }
    }

    std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        
        while (std::getline(ss, token, delimiter)) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }
        
        return tokens;
    }

    std::string toLowerCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    bool matchesPattern(const std::string& filename, const std::string& pattern) {
        try {
            // Convert glob pattern to regex
            std::string regexPattern = pattern;
            
            // Escape special regex characters except * and ?
            regexPattern = std::regex_replace(regexPattern, std::regex(R"([\[\](){}^$+\.|\\])"), R"(\$&)");
            
            // Convert glob patterns to regex
            regexPattern = std::regex_replace(regexPattern, std::regex(R"(\*)"), R"(.*)");
            regexPattern = std::regex_replace(regexPattern, std::regex(R"(\?)"), R"(.)");
            
            // Add anchors
            regexPattern = "^" + regexPattern + "$";
            
            std::regex regex(regexPattern, std::regex_constants::icase);
            return std::regex_match(filename, regex);
        } catch (const std::exception&) {
            // If regex fails, fall back to simple substring matching
            return toLowerCase(filename).find(toLowerCase(pattern)) != std::string::npos;
        }
    }

}
