cmake_minimum_required(VERSION 3.10)

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

# Find LCM dependencies
find_package(LCM REQUIRED)

set( LCM_MSG_HEADER 
    msg/imu_sensor_t.hpp 
    msg/revolute_servo_t.hpp 
    msg/sensors_t.hpp 
    msg/servos_t.hpp 
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



set( LCM_EXAMPLE_SOURCE 
    myDisplayWithLCM.cc 
)

set( LCM_EXAMPLE_HEADER 
    displayRT_parser.h 
    myDisplayWithLCM.h 
)

set( MINIYAML_HEADER 
    "${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml/Yaml.hpp"
)
set( MINIYAML_SOURCE 
    "${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml/Yaml.cpp"
)

add_executable(displayrt_example_lcm_node 
displayrt_example_lcm.cc
    ${LCM_EXAMPLE_SOURCE} 
    ${LCM_EXAMPLE_HEADER}
    ${MINIYAML_SOURCE}
    ${MINIYAML_HEADER}
    ${LCM_MSG_HEADER} 
)

target_link_libraries(displayrt_example_lcm_node
    Qt5::Core
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Gui
    ${DisplayRT_LIBRARY}
    ${QCustomPlot_LIBRARY}
    ${LCM_LIBRARY} 
    pthread
)

target_include_directories(displayrt_example_lcm_node
    PUBLIC 
    ${LCM_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}
    ${DisplayRT_INCLUDE_DIR} 
    ${QCustomPlot_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/../../thirdparty/mini-yaml/yaml 
)

set_target_properties(displayrt_example_lcm_node
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

