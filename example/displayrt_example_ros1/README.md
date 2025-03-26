# DisplayRT Example for ROS1
This is an example CMake project for DisplayRT with Robot Operating System (ROS1). 
The program is multithreaded with one thread publishing multiple messages on different topics through ROS1 and the other thread subscribing to the topics and plotting online the received message data.

The program is tested with Ubuntu 20.04 (ROS1 Noetic). 

# Dependencies (Ubuntu 20.04)
In addition to the [packages](../../README.md#dependencies) for DisplayRT, you need to install ROS1 by following the instructions from the [official tutorial](https://wiki.ros.org/noetic/Installation). 

# Compilation and Run
The easiest way is to run [setup.sh](./setup.sh) in the current project folder. It will generate ROS1 message headers and libraries and compile the project.  You can also manually compile it as a plain CMake project in /build directory. 
The generated executable is stored in /bin and message files in /build directory. 

Assume you are still in the current project folder, run ```./bin/displayrt_example_ros1_node -path-to-config ./config.yaml``` while ```roscore``` is executing meanwhile. 