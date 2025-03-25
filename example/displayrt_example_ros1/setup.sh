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

# check if

# check if DISPLAYRT_ROS2_WS environment variable is set to the current directory
if [ "$DISPLAYRT_ROS2_WS" != "$(pwd)" ]; then
    echo "DISPLAYRT_ROS2_WS environment variable is not set to the current directory."
    echo "export DISPLAYRT_ROS2_WS=$(pwd)" >> ~/.bashrc
else
    echo "DISPLAYRT_ROS2_WS environment variable is set to the current directory."
fi

# check if LD_LIBRARY_PATH includes \$DISPLAYRT_ROS2_WS/build
if [[ ":$LD_LIBRARY_PATH:" != *":$DISPLAYRT_ROS2_WS/build:"* ]]; then
    echo "LD_LIBRARY_PATH does not include \$DISPLAYRT_ROS2_WS/build."
    echo "export LD_LIBRARY_PATH=\$DISPLAYRT_ROS2_WS/build:\$LD_LIBRARY_PATH" >> ~/.bashrc
else
    echo "LD_LIBRARY_PATH includes \$DISPLAYRT_ROS2_WS/build."
fi


source ~/.bashrc

# Enter the /build directory
cd "$(pwd)/build"

# Compile the cmake project
cmake ..
make

# Exit
exit 0