cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0071 NEW)
message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_NAME_ROS2 ros2_example)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rcutils REQUIRED)

find_package(rosidl_cmake REQUIRED)
find_package(rosidl_typesupport_fastrtps_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME_ROS2}
  "types/IMU.msg"
  DEPENDENCIES std_msgs
  # If you want these to be installed automatically with the normal CMake
  # "install" command, do NOT set SKIP_INSTALL here.
)

install(
  DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp/${PROJECT_NAME_ROS2}/msg/
  DESTINATION
    msg
)

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

ament_target_dependencies(MultiwindowPlotWithParsedDisplayRTandROS2
    rclcpp
    std_msgs
    rcutils
)

target_link_libraries(MultiwindowPlotWithParsedDisplayRTandROS2  
    displayrt 
    pthread 
)

target_include_directories(MultiwindowPlotWithParsedDisplayRTandROS2 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    ${rclcpp_INCLUDE_DIRS}
    ${std_msgs_INCLUDE_DIRS} 
)

set_target_properties(MultiwindowPlotWithParsedDisplayRTandROS2
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

