#!/bin/bash

# Build script for File System Manager
# This script automates the build process

echo "Building File System Manager..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
    echo "Created build directory"
fi

# Change to build directory
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Check if CMake configuration was successful
if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build the project
echo "Building project..."
cmake --build . --config Release

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Executable: build/fsmanager"
    echo ""
    echo "To run the program:"
    echo "  ./build/fsmanager"
    echo ""
    echo "To run with example script:"
    echo "  ./build/fsmanager --script example_script.txt"
else
    echo "Build failed!"
    exit 1
fi

