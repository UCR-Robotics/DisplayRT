cmake_policy(SET CMP0057 NEW)
message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rcutils REQUIRED)

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

add_executable(MultiwindowPlotWithParsedDisplayRTandROS2 
MultiwindowPlotWithParsedDisplayRTandROS2.cc 
    ${ROS2_EXAMPLE_SOURCE} 
    ${ROS2_EXAMPLE_HEADER} 
)

target_link_libraries(MultiwindowPlotWithParsedDisplayRTandROS2  
    displayrt 
    rclcpp::rclcpp 
    # ${CMAKE_CURRENT_SOURCE_DIR} 
    pthread 
)

target_include_directories(MultiwindowPlotWithParsedDisplayRTandROS2 
    PUBLIC 
    # ${LCM_INCLUDE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    ${rclcpp_INCLUDE_DIRS}
    ${std_msgs_INCLUDE_DIRS} 
)

set_target_properties(MultiwindowPlotWithParsedDisplayRTandROS2
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

