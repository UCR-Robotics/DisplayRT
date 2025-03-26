# DisplayRT Example for ROS2
This is an example CMake project for DisplayRT with Robot Operating System (ROS2). 
The program is multithreaded with one thread publishing multiple messages on different topics through ROS2 and the other thread subscribing to the topics and plotting online the received message data.

The program is tested with Ubuntu 20.04 (ROS2 Foxy), Ubuntu 22.04 (ROS2 Humble), and Ubuntu 24.04 (ROS2 Jazzy).  

# Dependencies
In addition to the [packages](../../README.md#dependencies) for DisplayRT, you need to install ROS2 by following the instructions from the official tutorial. 

# Compilation and Run
The easiest way is to run [setup.sh](./setup.sh) in the current project folder. It will generate ROS2 message headers and libraries and compile the project.  You can also manually compile it as a plain CMake project in /build directory. 
The generated executable is stored in /bin and message files in the /build directory. 

Note that the environment variable ```LD_LIBRARY_PATH``` needs to include the absolute path to the /build directory. 

Assume you are still in the current project folder, run ```./bin/displayrt_example_ros2_node -path-to-config ./config.yaml```. 