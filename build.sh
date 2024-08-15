#!/bin/bash

# Define the build directory
BUILD_DIR="./build"

# Check if the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory does not exist. Creating it..."
    mkdir "$BUILD_DIR"
else
    echo "Build directory already exists."
fi

# Change to the build directory
cd "$BUILD_DIR"

# Run CMake and Make
echo "Running CMake..."
cmake ..

echo "Running Make..."
make
