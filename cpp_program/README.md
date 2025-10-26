# File System Manager

A powerful, modern C++ command-line file management tool built with C++17 and the standard filesystem library. This project demonstrates advanced C++ concepts including RAII, smart pointers, STL containers, async operations, and modern C++ features.

## Features

### Core Functionality
- **File Operations**: Create, read, write, copy, move, delete files and directories
- **Advanced Search**: Find files by name pattern, content, size, date, and extension
- **Batch Operations**: Perform operations on multiple files with progress tracking
- **Directory Management**: Navigate, list, and organize directory structures
- **File Information**: Display detailed file metadata and statistics

### Advanced Features
- **Pattern Matching**: Support for glob patterns and regex in file searches
- **Content Search**: Search within text files with line-by-line matching
- **Duplicate Detection**: Find duplicate files by content or size
- **Async Operations**: Non-blocking file operations for large datasets
- **Progress Tracking**: Real-time progress updates for batch operations
- **Cross-platform**: Works on Windows, macOS, and Linux

## Project Structure

```
cpp_program/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── include/                # Header files
│   ├── Common.h            # Common utilities and data structures
│   ├── FileManager.h       # Core file management class
│   ├── SearchEngine.h      # File search and pattern matching
│   ├── BatchOperations.h   # Batch file operations
│   └── CLI.h              # Command-line interface
└── src/                   # Source files
    ├── main.cpp           # Main entry point
    ├── Common.cpp         # Common utilities implementation
    ├── FileManager.cpp    # File management implementation
    ├── SearchEngine.cpp   # Search engine implementation
    ├── BatchOperations.cpp # Batch operations implementation
    └── CLI.cpp           # CLI implementation
```

## Building the Project

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16 or later
- Standard library with filesystem support

### Build Instructions

1. **Clone or navigate to the project directory:**
   ```bash
   cd cpp_program
   ```

2. **Create build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake:**
   ```bash
   cmake ..
   ```

4. **Build the project:**
   ```bash
   cmake --build .
   ```

5. **Run the executable:**
   ```bash
   ./fsmanager
   ```

### Alternative Build (without CMake)
```bash
g++ -std=c++17 -o fsmanager src/*.cpp -I include
```

## Usage

### Interactive Mode
Start the program without arguments to enter interactive mode:
```bash
./fsmanager
```

### Command Line Arguments
```bash
./fsmanager [options] [initial_path]
```

**Options:**
- `--help, -h`: Show help message
- `--version, -v`: Show version information
- `--script <file>`: Run commands from script file

**Examples:**
```bash
./fsmanager /home/user/documents    # Start in specific directory
./fsmanager --script commands.txt   # Run script file
./fsmanager --help                  # Show help
```

## Available Commands

### Navigation Commands
| Command | Description | Example |
|---------|-------------|---------|
| `pwd` | Print current directory | `pwd` |
| `cd <path>` | Change directory | `cd /home/user` |
| `ls [options]` | List files and directories | `ls -la` |
| `tree [depth]` | Show directory tree | `tree 3` |

### File Operations
| Command | Description | Example |
|---------|-------------|---------|
| `touch <file>` | Create empty file | `touch newfile.txt` |
| `mkdir <dir>` | Create directory | `mkdir newdir` |
| `rm <file>` | Delete file | `rm oldfile.txt` |
| `cp <src> <dest>` | Copy file | `cp file.txt backup.txt` |
| `mv <src> <dest>` | Move/rename file | `mv old.txt new.txt` |
| `cat <file>` | Display file content | `cat readme.txt` |
| `echo <text> > <file>` | Write text to file | `echo "Hello" > test.txt` |

### Search Commands
| Command | Description | Example |
|---------|-------------|---------|
| `find <pattern>` | Find files by name pattern | `find *.txt` |
| `grep <term> [file]` | Search content in files | `grep "error" *.log` |
| `search <options>` | Advanced search | `search -name "*.cpp" -size 1000-5000` |

### Batch Operations
| Command | Description | Example |
|---------|-------------|---------|
| `batch copy <pattern> <dest>` | Copy files by pattern | `batch copy *.jpg photos/` |
| `batch move <pattern> <dest>` | Move files by pattern | `batch move *.tmp temp/` |
| `batch delete <pattern>` | Delete files by pattern | `batch delete *.bak` |

### Utility Commands
| Command | Description | Example |
|---------|-------------|---------|
| `size <path>` | Show file/directory size | `size largefile.zip` |
| `stats` | Show directory statistics | `stats` |
| `clear` | Clear screen | `clear` |
| `help` | Show help | `help` |
| `exit` | Exit program | `exit` |

## Advanced Features

### Pattern Matching
The search engine supports various pattern types:
- **Glob patterns**: `*.txt`, `file?.log`, `[0-9]*.dat`
- **Regex patterns**: Enable with regex mode for complex patterns
- **Case sensitivity**: Configurable case-sensitive/insensitive matching

### Batch Operations
- **Progress tracking**: Real-time progress updates
- **Error handling**: Detailed error reporting for failed operations
- **Async support**: Non-blocking operations for large file sets
- **Cancellation**: Ability to cancel long-running operations

### File Information
- **Metadata display**: File size, modification date, type
- **Directory statistics**: File counts, total sizes, directory trees
- **Content analysis**: Text file detection, binary file handling

## Technical Details

### Design Patterns
- **RAII**: Resource management through constructors/destructors
- **Command Pattern**: CLI command handling
- **Observer Pattern**: Progress callbacks and notifications
- **Strategy Pattern**: Different search algorithms

### Modern C++ Features
- **std::filesystem**: Cross-platform file system operations
- **std::async**: Asynchronous operations
- **Smart pointers**: Automatic memory management
- **STL containers**: Efficient data structures
- **Lambda expressions**: Functional programming support
- **Move semantics**: Performance optimization

### Error Handling
- **Exception safety**: Strong exception safety guarantees
- **Graceful degradation**: Continue operation despite individual failures
- **Detailed error reporting**: Comprehensive error messages and logging

## Examples

### Basic File Operations
```bash
# Create a new file
touch example.txt

# Write content to file
echo "Hello, World!" > example.txt

# Display file content
cat example.txt

# Copy file
cp example.txt backup.txt

# List files with details
ls -l
```

### Advanced Search
```bash
# Find all C++ files
find "*.cpp"

# Search for specific content
grep "TODO" *.cpp

# Find large files
search -size 1000000-

# Find files modified today
search -date $(date +%Y-%m-%d)
```

### Batch Operations
```bash
# Copy all images to photos directory
batch copy "*.jpg" photos/
batch copy "*.png" photos/

# Clean up temporary files
batch delete "*.tmp"
batch delete "*.bak"

# Organize files by extension
batch move "*.pdf" documents/
batch move "*.doc" documents/
```

## Performance Considerations

- **Memory efficiency**: Minimal memory footprint for large directory operations
- **I/O optimization**: Efficient file reading and writing
- **Caching**: Directory listing cache for improved performance
- **Async operations**: Non-blocking operations for better responsiveness

## Future Enhancements

- **GUI interface**: Graphical user interface option
- **Network operations**: Remote file system support
- **Compression**: Built-in file compression/decompression
- **Encryption**: File encryption and decryption
- **Cloud integration**: Cloud storage service support
- **Plugin system**: Extensible architecture for custom operations

## Contributing

This project serves as a demonstration of modern C++ programming techniques. Feel free to:
- Report bugs and issues
- Suggest new features
- Submit improvements
- Use as a learning resource

## License

This project is provided as an educational example. Feel free to use and modify for learning purposes.

## Author

Created as a demonstration of advanced C++ programming concepts and modern software design patterns.
