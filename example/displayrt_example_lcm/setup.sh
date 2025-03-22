#!/bin/bash

# Note: only run this script when you know what you are doing.
# run it once from the current directory of this sub-project as ./setup.sh

source ~/.bashrc

# create /build directory if it does not exist
if [ ! -d "$(pwd)/build" ]; then
    # If not, create the /build directory
    mkdir "$(pwd)/build"
    echo "/build directory created."
else
    echo "/build directory already exists."
fi

# generate message types
./generate_lcm_msg_types.sh

# Enter the /build directory
cd "$(pwd)/build"

# Compile the cmake project
cmake ..
make

# return to the original path
cd ..
echo -e "\e[32mReturned to $(pwd).\e[0m"
echo -e "\e[32mBuild completed.\e[0m"

# Exit
exit 0