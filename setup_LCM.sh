#!/bin/bash

# Note: only run this script when you know what you are doing.
# Setup LCM v1.4.0 


# check if liblcm.so file exists in the /usr/local/lib directory
if [ -f "/usr/local/lib/liblcm.so" ]; then
    echo -e "\e[32mThe liblcm.so file exists in the /usr/local/lib directory.\e[0m"
    exit 0
else
    echo -e "\e[32mThe liblcm.so file does not exist in the /usr/local/lib directory. Start compiling the LCM library.\e[0m"
fi

# Dependencies
sudo apt update
sudo apt remove --purge openjdk-11-*
sudo apt install build-essential libglib2.0-dev python3-dev openjdk-8-jdk doxygen git

# Safe the current path for later use
current_path=$(pwd)

# Go to home directory and git clone the LCM library v.1.4.0 from GitHub
cd ~
git clone https://github.com/lcm-proj/lcm.git --branch v1.4.0 --single-branch
cd lcm
# check if build directory exists
if [ ! -d "$(pwd)/build" ]; then
    # If not, create the /build directory
    mkdir build
    echo "/build directory created."
else
    echo "/build directory already exists."
fi
# mkdir build
cd build
cmake ..
make
sudo make install

# Return to the original path
cd $current_path
echo -e "\e[32mReturned to $current_path.\e[0m"

# Exit the script
exit 0
