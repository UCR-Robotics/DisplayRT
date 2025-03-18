cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0071 NEW)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# project(ros2foxy)
get_filename_component(PROJECT ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT})

message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

# Find ROS 2 dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rosidl_typesupport_fastrtps_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

# Add custom message files
set(msg_files
    types/IMU.msg
)

# Generate ROS 2 interfaces
rosidl_generate_interfaces(${PROJECT_NAME}
    ${msg_files}
    DEPENDENCIES std_msgs
)

# Add the executable
set( ROS2_EXAMPLE_SOURCE 
    # myDisplayWithLCM.cc 
)

set( ROS2_EXAMPLE_HEADER 
    # ../displayRT_parser.h 
    # myDisplayWithLCM.h 
    # msg/imu_sensor_t.hpp 
    # msg/revolute_servo_t.hpp 
    # msg/sensors_t.hpp 
    # msg/servos_t.hpp 
)

add_executable(MultiwindowPlotWithParsedDisplayRTandROS2Foxy
    MultiwindowPlotWithParsedDisplayRTandROS2Foxy.cc 
    ${ROS2_EXAMPLE_SOURCE} 
    ${ROS2_EXAMPLE_HEADER} 
)

# Link the shared library and ROS 2 dependencies
target_link_libraries(MultiwindowPlotWithParsedDisplayRTandROS2Foxy
    displayrt
    pthread
    ros2foxy__rosidl_typesupport_cpp
    ros2foxy__rosidl_typesupport_fastrtps_cpp
    ros2foxy__rosidl_typesupport_introspection_cpp
)

ament_target_dependencies(MultiwindowPlotWithParsedDisplayRTandROS2Foxy
    rclcpp
    std_msgs
)

# Include directories
target_include_directories(MultiwindowPlotWithParsedDisplayRTandROS2Foxy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${rclcpp_INCLUDE_DIRS} 
    ${std_msgs_INCLUDE_DIRS} 
)

set_target_properties(MultiwindowPlotWithParsedDisplayRTandROS2Foxy
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

# # Install the executable
# install(TARGETS MultiwindowPlotWithParsedDisplayRTandROS2Foxy
#     DESTINATION lib/${PROJECT_NAME}
# )

# Install the message files at the /msg directory of the current package
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp/${PROJECT_NAME}/msg/
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/msg
)

# install(
#     FILES ${CMAKE_BINARY_DIR}/install_manifest.txt
#     DESTINATION ${CMAKE_INSTALL_PREFIX}
# )

# Export dependencies
ament_export_dependencies(rosidl_default_runtime)
ament_package()





# ## Original CMakeLists.txt
# cmake_policy(SET CMP0057 NEW)
# cmake_policy(SET CMP0054 NEW)
# cmake_policy(SET CMP0071 NEW)
# message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})
# set(PROJECT_NAME_ROS2 displayrtros2foxy)

# find_package(ament_cmake REQUIRED)
# find_package(rclcpp REQUIRED)
# find_package(std_msgs REQUIRED)
# find_package(rcutils REQUIRED)

# find_package(rosidl_cmake REQUIRED)
# find_package(rosidl_default_generators REQUIRED)
# find_package(rosidl_typesupport_fastrtps_cpp REQUIRED)
# find_package(rosidl_typesupport_introspection_cpp REQUIRED)

# set(msg_files
#     types/IMU.msg
# )

# rosidl_generate_interfaces(${PROJECT_NAME_ROS2}
#     ${msg_files} 
#     DEPENDENCIES std_msgs
# )

# ament_export_dependencies(rosidl_default_runtime)

# # rosidl_generate_interfaces(${PROJECT_NAME_ROS2}
# #   "types/IMU.msg"
# #   DEPENDENCIES std_msgs
# #   # If you want these to be installed automatically with the normal CMake
# #   # "install" command, do NOT set SKIP_INSTALL here.
# # )

# # install(
# #   DIRECTORY
# #     ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp/${PROJECT_NAME_ROS2}/msg/
# #   DESTINATION
# #     msg
# # )

# set( ROS2_EXAMPLE_SOURCE 
#     # myDisplayWithLCM.cc 
# )

# set( ROS2_EXAMPLE_HEADER 
#     # ../displayRT_parser.h 
#     # myDisplayWithLCM.h 
#     # msg/imu_sensor_t.hpp 
#     # msg/revolute_servo_t.hpp 
#     # msg/sensors_t.hpp 
#     # msg/servos_t.hpp 
# )

# add_executable(MultiwindowPlotWithParsedDisplayRTandROS2Foxy 
# MultiwindowPlotWithParsedDisplayRTandROS2Foxy.cc 
#     ${ROS2_EXAMPLE_SOURCE} 
#     ${ROS2_EXAMPLE_HEADER} 
# )

# ament_target_dependencies(MultiwindowPlotWithParsedDisplayRTandROS2Foxy
#     rclcpp
#     std_msgs
#     rcutils
# )

# target_link_libraries(MultiwindowPlotWithParsedDisplayRTandROS2Foxy  
#     displayrt 
#     pthread 
# )

# target_include_directories(MultiwindowPlotWithParsedDisplayRTandROS2Foxy 
#     PUBLIC 
#     ${CMAKE_CURRENT_SOURCE_DIR} 
#     ${CMAKE_CURRENT_SOURCE_DIR}/.. 
#     ${rclcpp_INCLUDE_DIRS}
#     ${std_msgs_INCLUDE_DIRS} 
# )

# set_target_properties(MultiwindowPlotWithParsedDisplayRTandROS2Foxy
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
# )

# # Export dependencies
# ament_export_dependencies(rosidl_default_runtime)
# ament_package()

