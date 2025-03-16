cmake_policy(SET CMP0057 NEW)
cmake_minimum_required(VERSION 3.0)

# get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
set(PROJECT "displayrt")
set(PROJECT_NAME ${PROJECT})
project(${PROJECT})

set(DISPLAY_RT_DIR ${CMAKE_SOURCE_DIR})
message(STATUS "Entering: " ${DISPLAY_RT_DIR})

# project(sciq_util)
message(STATUS "Build project: " ${PROJECT_NAME})
message(STATUS "If cross-compiling: " ${CMAKE_CROSSCOMPILING})
message(STATUS "Target platform: " ${CMAKE_SYSTEM_NAME})
message(STATUS "Target system root: " ${CMAKE_SYSROOT})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_AUTOMOC ON)

set(DEBUG_LEVEL "0") # debug level, 0: no debug, 1: basic debug, 2: detailed debug
add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
message(STATUS "Debug Level: " ${DEBUG_LEVEL})

# Libraries
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


# QCustomPlot
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Gui REQUIRED)

find_package(QCustomPlot REQUIRED)
# add_definitions(-DQCUSTOMPLOT_USE_LIBRARY)

set(LIB_DISPLAYRT_HEADERS 
    "${DISPLAY_RT_DIR}/include/displayRT.h" 
)

set(LIB_DISPLAYRT_SOURCES 
    "${DISPLAY_RT_DIR}/include/displayRT.cc" 
)

set(LIB_MINIYAML_HEADERS 
    "${DISPLAY_RT_DIR}/thirdparty/mini-yaml/yaml/Yaml.hpp"
)
set(LIB_MINIYAML_SOURCES 
    "${DISPLAY_RT_DIR}/thirdparty/mini-yaml/yaml/Yaml.cpp"
)

# Create Library
add_library(displayrt SHARED 
    ${LIB_DISPLAYRT_HEADERS} ${LIB_DISPLAYRT_SOURCES} 
    ${LIB_MINIYAML_HEADERS} ${LIB_MINIYAML_SOURCES}
)

target_link_libraries(displayrt PUBLIC 
    Qt5::Widgets Qt5::Core Qt5::PrintSupport Qt5::Gui 
    ${QCustomPlot_LIBRARY}
    ${LCM_LIBRARY}
)

target_include_directories(displayrt PUBLIC 
    ${DISPLAY_RT_DIR}/include 
    ${DISPLAY_RT_DIR}/thirdparty/mini-yaml/yaml 
    ${QCustomPlot_INCLUDE_DIR} 
) 

set_target_properties(displayrt
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/lib"
)


# Add subdirectories
# add_subdirectory(test)
add_subdirectory(example)
# add_subdirectory(src)