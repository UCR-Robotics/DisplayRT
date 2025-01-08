#!/bin/bash

# QT5 
sudo apt install qtbase5-dev qt5-qmake qtbase5-dev-tools libqt5widgets5

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

# Compile the cmake project
cmake ..
make