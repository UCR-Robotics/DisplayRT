#!/bin/bash

# run it from the root directory of the project as ./setup.sh

# submodules
git submodule update --init --recursive

# QT5 
sudo apt install g++ make cmake qtbase5-dev qt5-qmake qtbase5-dev-tools libqt5widgets5 qtwayland5

# compile QCustomPlot by running setup_QCustomPlot.sh
./setup_QCustomPlot.sh

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

# Exit
exit 0