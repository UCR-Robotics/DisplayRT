# DisplayRT Example for LCM

<div style="text-align: left;">
  <img src="../../doc/DisplayRT_example_lcm.gif" alt="DisplayRT Example with LCM" width="800"/>
</div>


This is an example CMake project for DisplayRT with Lightweight Communications and Marshalling (LCM). 
The program is multithreaded with one thread publishing multiple messages on different topics through LCM and the other thread subscribing to the topics and plotting online the received message data.  

# Dependencies
In addition to the [packages](../../README.md#dependencies) for DisplayRT, you need to install LCM through apt or building from source. 
To do so, follow the instructions from the [official tutorial](https://lcm-proj.github.io/lcm/content/install-instructions.html#installing-lcm) or run [setup_LCM.sh](../../util/setup_LCM.sh)  

# Compilation
The easiest way is to run [setup.sh](./setup.sh) in the current project folder. It will generate LCM message headers and compile the project.  
You can also manually compile it as a normal CMake project after running [generate_lcm_msg_types.sh](./generate_lcm_msg_types.sh) to generate message header files.  