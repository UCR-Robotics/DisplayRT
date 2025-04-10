cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "CMake must be run from the project folder: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# check ROS2 environment variables
if(NOT DEFINED ENV{ROS_DISTRO})
message(FATAL_ERROR "No ROS2 environment detected")
elseif( $ENV{ROS_DISTRO} STREQUAL "foxy")
    message(STATUS "ROS2 Foxy detected")
    add_definitions(-DROS2_FOXY)
elseif( $ENV{ROS_DISTRO} STREQUAL "galactic")
    message(STATUS "ROS2 Galactic detected")
    add_definitions(-DROS2_GALACTIC)
elseif( $ENV{ROS_DISTRO} STREQUAL "humble")
    message(STATUS "ROS2 Humble detected")
    add_definitions(-DROS2_HUMBLE)
elseif( $ENV{ROS_DISTRO} STREQUAL "jazzy")
    message(STATUS "ROS2 Jazzy detected")
    add_definitions(-DROS2_JAZZY)
else()
    message(FATAL_ERROR "Unsupported ROS2 distribution: " $ENV{ROS_DISTRO})
endif()

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
find_package(std_msgs REQUIRED)
find_package(rosidl_typesupport_fastrtps_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

# Generate custom message headers
set(msg_files
    types/IMU.msg 
    types/RevoluteServo.msg
    types/Sensors.msg 
    types/Servos.msg
)

rosidl_generate_interfaces(${PROJECT_NAME}
    ${msg_files}
    DEPENDENCIES std_msgs
)

# Find DisplayRT dependencies
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(QCustomPlot REQUIRED)
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
)

ament_target_dependencies(displayrt_example_ros2_node
    rclcpp
    std_msgs
    rosidl_typesupport_fastrtps_cpp
)

add_dependencies(displayrt_example_ros2_node
    ${PROJECT_NAME}__rosidl_typesupport_cpp
    ${PROJECT_NAME}__rosidl_typesupport_fastrtps_cpp
    ${PROJECT_NAME}__rosidl_typesupport_introspection_cpp
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


ament_export_dependencies(rosidl_default_runtime)

ament_package()






