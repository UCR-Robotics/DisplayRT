#!/bin/bash

# Safe the current path for later use
current_path=$(pwd)

# Check if the /thirdparty/QCustomPlot/ directory exists in the current path. If not, exit with warning. If so, enter the directory.
if [ ! -d "$(pwd)/thirdparty/QCustomPlot" ]; then
    echo -e "\e[31mThe /thirdparty/QCustomPlot/ directory does not exist in the current path. Please make sure you are in the correct directory.\e[0m"
    exit 1
else
    cd "$(pwd)/thirdparty/QCustomPlot"
    echo -e "\e[32mEntered $(pwd).\e[0m"
fi

# Check if the /build directory exists in the current path
if [ ! -d "$(pwd)/build" ]; then
    # If not, create the /build directory
    mkdir "$(pwd)/build"
    echo -e "\e[32m/build directory created.\e[0m"
else
    echo -e "\e[32m/build directory already exists.\e[0m"
fi

# Enter the /build directory
cd "$(pwd)/build"
echo -e "\e[32mEntered $(pwd).\e[0m"

# Run qmake and make the project
qmake ..
make

# Return to the original path
cd $current_path
echo -e "\e[32mReturned to $current_path.\e[0m"

# Exit the script
exit 0
