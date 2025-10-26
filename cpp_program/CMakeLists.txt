cmake_minimum_required(VERSION 3.16)
project(FileSystemManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Common.cpp
    src/FileManager.cpp
    src/SearchEngine.cpp
    src/BatchOperations.cpp
    src/CLI.cpp
)

# Header files
set(HEADERS
    include/FileManager.h
    include/SearchEngine.h
    include/BatchOperations.h
    include/CLI.h
    include/Common.h
)

# Create executable
add_executable(fsmanager ${SOURCES} ${HEADERS})

# Optional: Create a library
add_library(fsmanager_lib STATIC ${SOURCES} ${HEADERS})

# Installation
install(TARGETS fsmanager DESTINATION bin)
