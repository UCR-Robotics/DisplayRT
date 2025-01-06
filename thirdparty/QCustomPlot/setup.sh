#!/bin/bash

# Check if the /build directory exists in the current path
if [ ! -d "$(pwd)/build" ]; then
    # If not, create the /build directory
    mkdir "$(pwd)/build"
    echo "/build directory created."
else
    echo "/build directory already exists."
fi

# Enter the /build directory
cd "$(pwd)/build"

# Run qmake and make the project
qmake ..
make

