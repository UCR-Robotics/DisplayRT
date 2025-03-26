#!/bin/bash

# Note: only run this script when you know what you are doing.
# Install LCM through apt on Ubuntu 24.04 LTS, or setup LCM v1.4.0 for Ubuntu 20.04 LTS and 22.04 LTS

# Ask for the user's confirmation and wait for a response with enter key
echo -e "\e[31mThis script will install the LCM library.\e[0m"
read -p "Do you want to continue? (y/n) " REPLY
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo -e "\e[31mInstallation aborted.\e[0m"
    exit 1
fi

# Check if liblcm.so exists
liblcm_path=$(find /usr -name "liblcm.so" 2>/dev/null)

if [ -z "$liblcm_path" ]; then
    echo -e "\e[31mThe liblcm.so file was not found in /usr. Proceeding with installation...\e[0m"
    # Proceed with installation steps
else
    echo -e "\e[32mThe liblcm.so file already exists at: $liblcm_path\e[0m"
    exit 0
fi

# Function to build LCM v1.4.0 from source
build_lcm_from_source() {
# Build from source
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

# set up the environment variables
echo "export LCM_INSTALL_PREFIX=/usr/local" >> ~/.bashrc
echo "export LCM_LIBRARY_DIR=/usr/local/lib" >> ~/.bashrc
echo "export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$LCM_INSTALL_PREFIX" >> ~/.bashrc
echo "export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$LCM_LIBRARY_DIR/pkgconfig" >> ~/.bashrc
source ~/.bashrc
echo -e "\e[32mLCM library v1.4.0 installed.\e[0m"

# Return to the original path
cd $current_path
echo -e "\e[32mReturned to $current_path.\e[0m"
}


# check Ubuntu version, use apt if Ubuntu 24.04 LTS
if [ "$(lsb_release -rs)" == "24.04" ]; then
    echo -e "\e[32mUbuntu 24.04 LTS detected. Using apt to install LCM library.\e[0m"
    sudo apt install liblcm-dev liblcm-java python3-lcm 
    exit 0
elif [ "$(lsb_release -rs)" == "20.04" ] || [ "$(lsb_release -rs)" == "22.04" ]; then
    echo -e "\e[32mUbuntu 20.04 LTS or 22.04 LTS detected. Building LCM library v1.4.0 from source.\e[0m"
    build_lcm_from_source
    exit 0
else
    echo -e "\e[31mError: This script only supports Ubuntu 20.04, 22.04, and 24.04.\e[0m"
    exit 1
fi

# In terminal, ask for the user's confirmation
echo -e "\e[31mThis script will install the LCM library v1.4.0 from source.\e[0m"
read -p "Do you want to continue? (y/n) "  REPLY
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo -e "\e[31mInstallation aborted.\e[0m"
    exit 1
fi


# # Build from source
# # Dependencies
# sudo apt update
# sudo apt remove --purge openjdk-11-*
# sudo apt install build-essential libglib2.0-dev python3-dev openjdk-8-jdk doxygen git

# # Safe the current path for later use
# current_path=$(pwd)

# # Go to home directory and git clone the LCM library v.1.4.0 from GitHub
# cd ~
# git clone https://github.com/lcm-proj/lcm.git --branch v1.4.0 --single-branch
# cd lcm
# # check if build directory exists
# if [ ! -d "$(pwd)/build" ]; then
#     # If not, create the /build directory
#     mkdir build
#     echo "/build directory created."
# else
#     echo "/build directory already exists."
# fi
# # mkdir build
# cd build
# cmake ..
# make
# sudo make install

# # set up the environment variables
# echo "export LCM_INSTALL_PREFIX=/usr/local" >> ~/.bashrc
# echo "export LCM_LIBRARY_DIR=/usr/local/lib" >> ~/.bashrc
# echo "export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$LCM_INSTALL_PREFIX" >> ~/.bashrc
# echo "export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$LCM_LIBRARY_DIR/pkgconfig" >> ~/.bashrc
# source ~/.bashrc
# echo -e "\e[32mLCM library v1.4.0 installed.\e[0m"



# # Return to the original path
# cd $current_path
# echo -e "\e[32mReturned to $current_path.\e[0m"

# Exit the script
exit 0
