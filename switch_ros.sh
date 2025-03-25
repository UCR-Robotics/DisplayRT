#!/bin/bash

# This script switches between ROS 1 Noetic and ROS 2 Foxy environments.
# Note: Only run this script when you know what you are doing.
# Usage: source switch_ros.sh [ros1|ros2]
# Example: source switch_ros.sh ros1
# This script assumes that you have both ROS 1 Noetic and ROS 2 Foxy installed on your system.
# It will set the appropriate environment variables and paths for the selected ROS version.
# It will also remove paths related to the other ROS version to avoid conflicts.
# Make sure to run this script in a bash shell.

# check ubuntu version, only accept ubuntu 20.04
if [ "$(lsb_release -rs)" != "20.04" ]; then
    # red warning
    echo -e "\033[31mError: This script only supports Ubuntu 20.04.\033[0m"
    exit 1
fi

# Function to remove paths related to the other ROS version
remove_other_ros_paths() {
    local ros_version_path=$1
    export LD_LIBRARY_PATH=$(echo $LD_LIBRARY_PATH | tr ':' '\n' | grep -v "$ros_version_path" | tr '\n' ':' | sed 's/:$//')
    export PATH=$(echo $PATH | tr ':' '\n' | grep -v "$ros_version_path" | tr '\n' ':' | sed 's/:$//')
    export PYTHONPATH=$(echo $PYTHONPATH | tr ':' '\n' | grep -v "$ros_version_path" | tr '\n' ':' | sed 's/:$//')
}

# Function to switch to ROS 1 Noetic
use_ros1() {
    echo "Switching to ROS 1 Noetic..."
    unset ROS_DISTRO ROS_PACKAGE_PATH AMENT_PREFIX_PATH COLCON_PREFIX_PATH
    source /opt/ros/noetic/setup.bash

    # Remove ROS 2 Foxy paths
    remove_other_ros_paths "/opt/ros/foxy"
    remove_other_ros_paths "ros2"

    # Add ROS 1 Noetic paths
    export LD_LIBRARY_PATH=/opt/ros/noetic/lib:$LD_LIBRARY_PATH
    export PATH=/opt/ros/noetic/bin:$PATH
    export PYTHONPATH=/opt/ros/noetic/lib/python3/dist-packages:$PYTHONPATH

    echo "Environment set for ROS 1 Noetic."
}

# Function to switch to ROS 2 Foxy
use_ros2() {
    echo "Switching to ROS 2 Foxy..."
    unset ROS_DISTRO ROS_PACKAGE_PATH AMENT_PREFIX_PATH COLCON_PREFIX_PATH
    source /opt/ros/foxy/setup.bash

    # Remove ROS 1 Noetic paths
    remove_other_ros_paths "/opt/ros/noetic"
    remove_other_ros_paths "ros1"

    # Add ROS 2 Foxy paths
    export LD_LIBRARY_PATH=/opt/ros/foxy/lib:$LD_LIBRARY_PATH
    export PATH=/opt/ros/foxy/bin:$PATH
    export PYTHONPATH=/opt/ros/foxy/lib/python3/dist-packages:$PYTHONPATH

    # Additional setup
    export ROS_DOMAIN_ID=0
    export ROS_LOCALHOST_ONLY=1
    export RMW_IMPLEMENTATION=rmw_fastrtps_cpp

    echo "Environment set for ROS 2 Foxy."
}

# Check the input argument
if [ "$1" == "ros1" ]; then
    use_ros1
elif [ "$1" == "ros2" ]; then
    use_ros2
else
    echo "Usage: source switch_ros.sh [ros1|ros2]"
    echo "Example: source switch_ros.sh ros1"
fi

