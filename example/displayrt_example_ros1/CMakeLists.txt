cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "CMake must be run from the project folder: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# check ROS1 environment variables
if(NOT DEFINED ENV{ROS_DISTRO})
message(FATAL_ERROR "No ROS1 environment detected")
elseif( $ENV{ROS_DISTRO} STREQUAL "melodic")
    message(STATUS "ROS1 Melodic detected")
    add_definitions(-DROS1_MELODIC)
elseif( $ENV{ROS_DISTRO} STREQUAL "noetic")
    message(STATUS "ROS1 Noetic detected")
    add_definitions(-DROS1_NOETIC)
else()
    message(FATAL_ERROR "Unsupported ROS1 distribution: " $ENV{ROS_DISTRO})
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

# Find ROS 1 dependencies
find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    message_generation
)

# Generate custom message headers and libs
add_message_files(
    DIRECTORY types
    FILES
    IMU.msg
    RevoluteServo.msg
    Sensors.msg
    Servos.msg
)

generate_messages(
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
set( ROS1_EXAMPLE_SOURCE 
    myDisplayWithROS1.cc
)

set( ROS1_EXAMPLE_HEADER 
    displayRT_parser.h 
    myDisplayWithROS1.h
)

set( MINIYAML_HEADER 
    "${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml/Yaml.hpp"
)
set( MINIYAML_SOURCE 
    "${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml/Yaml.cpp"
)

add_executable(displayrt_example_ros1_node
    displayrt_example_ros1.cc 
    ${ROS1_EXAMPLE_SOURCE} 
    ${ROS1_EXAMPLE_HEADER} 
    ${MINIYAML_HEADER}
    ${MINIYAML_SOURCE}
)

target_include_directories(displayrt_example_ros1_node PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${DisplayRT_INCLUDE_DIR} 
    ${QCustomPlot_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${catkin_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}/devel/include
)

target_link_libraries(displayrt_example_ros1_node
    Qt5::Core
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Gui
    ${DisplayRT_LIBRARY}
    ${QCustomPlot_LIBRARY}
    pthread
    ${catkin_LIBRARIES}
)

add_dependencies(displayrt_example_ros1_node
    ${${PROJECT_NAME}_EXPORTED_TARGETS}
    ${catkin_EXPORTED_TARGETS}
)

set_target_properties(displayrt_example_ros1_node
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)