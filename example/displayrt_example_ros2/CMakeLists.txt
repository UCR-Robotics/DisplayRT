cmake_minimum_required(VERSION 3.10)
# cmake_policy(SET CMP0071 NEW)


get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})
# set(PROJECT_NAME ${PROJECT})
# project(${PROJECT_NAME})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "CMake must be run from the project folder: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Build project: " ${PROJECT_NAME})
message(STATUS "If cross-compiling: " ${CMAKE_CROSSCOMPILING})
message(STATUS "Target platform: " ${CMAKE_SYSTEM_NAME})
message(STATUS "Target system root: " ${CMAKE_SYSROOT})

set(DEBUG_LEVEL "0") # debug level, 0: no debug, 1: basic debug, 2: detailed debug
add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
message(STATUS "Debug Level: " ${DEBUG_LEVEL})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_AUTOMOC ON)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Find ROS 2 dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
# find_package(rcutils REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_typesupport_fastrtps_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

# Add custom message files
set(msg_files
    types/IMU.msg 
    types/RevoluteServo.msg
    types/Sensors.msg 
    types/Servos.msg
)

# Generate ROS 2 interfaces
rosidl_generate_interfaces(${PROJECT_NAME}
    ${msg_files}
    DEPENDENCIES std_msgs
)


# Find DisplayRT dependencies
# QT and QCustomPlot
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(QCustomPlot REQUIRED)

# Add the DisplayRT library
find_package(DisplayRT REQUIRED)

# Add the executable
set( ROS2_EXAMPLE_SOURCE 
    myDisplayWithROS2.cc
)

set( ROS2_EXAMPLE_HEADER 
    displayRT_parser.h 
    myDisplayWithROS2.h
)

set( MINIYAML_HEADER 
    "${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml/Yaml.hpp"
)
set( MINIYAML_SOURCE 
    "${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml/Yaml.cpp"
)

add_executable(displayrt_example_ros2_node
    displayrt_example_ros2.cc 
    ${ROS2_EXAMPLE_SOURCE} 
    ${ROS2_EXAMPLE_HEADER} 
    ${MINIYAML_HEADER}
    ${MINIYAML_SOURCE}
)

add_dependencies(displayrt_example_ros2_node
    # ${PROJECT_NAME}
    ${PROJECT_NAME}__rosidl_typesupport_cpp
    ${PROJECT_NAME}__rosidl_typesupport_fastrtps_cpp
    ${PROJECT_NAME}__rosidl_typesupport_introspection_cpp
)

# rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")

# Link the shared library and ROS 2 dependencies
target_link_libraries(displayrt_example_ros2_node
    Qt5::Core
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Gui
    ${DisplayRT_LIBRARY}
    ${QCustomPlot_LIBRARY}
    pthread
    ${PROJECT_NAME}__rosidl_typesupport_cpp
    ${PROJECT_NAME}__rosidl_typesupport_fastrtps_cpp
    ${PROJECT_NAME}__rosidl_typesupport_introspection_cpp
    # ${PROJECT_NAME}
    # ${cpp_typesupport_target}
)

ament_target_dependencies(displayrt_example_ros2_node
    rclcpp
    std_msgs
    # cpp_typesupport_target
    rosidl_typesupport_fastrtps_cpp
    # ${PROJECT_NAME}
    # ${PROJECT_NAME}__rosidl_typesupport_cpp
    # ${PROJECT_NAME}__rosidl_typesupport_fastrtps_cpp
    # ${PROJECT_NAME}__rosidl_typesupport_introspection_cpp
)

# Include directories
target_include_directories(displayrt_example_ros2_node PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${DisplayRT_INCLUDE_DIR} 
    ${QCustomPlot_INCLUDE_DIR}
    ${rclcpp_INCLUDE_DIRS} 
    ${std_msgs_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml 
    ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(displayrt_example_ros2_node
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# set_target_properties(displayrt_example_ros2_node 
#     PROPERTIES
#     BUILD_RPATH "${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp;${CMAKE_CURRENT_BINARY_DIR}"
#     INSTALL_RPATH "${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp;${CMAKE_CURRENT_BINARY_DIR}"
#     CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE
# )

# # Install the executable
# install(TARGETS displayrt_example_ros2_node
#     DESTINATION lib/${PROJECT_NAME}
# )

# Install the message files at the /msg directory of the current package
# install(
#     DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp/${PROJECT_NAME}/msg/
#     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/msg
# )

# install(
#     FILES ${CMAKE_BINARY_DIR}/install_manifest.txt
#     DESTINATION ${CMAKE_INSTALL_PREFIX}
# )

# Export dependencies
ament_export_dependencies(rosidl_default_runtime)

ament_package()






