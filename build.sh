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

# Run CMake
echo "Running CMake..."
if [ "$1" == "-db" ]; then
    echo "Created Xcode project for debugging."
    cmake -G Xcode ..
else
    # Need to add a check for xcode proj here to delete that build and redo
    echo "Default generator running."
    cmake ..

    # Make binary
    echo "Running Make..."
    make
fi


