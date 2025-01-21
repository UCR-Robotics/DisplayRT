cmake_minimum_required(VERSION 3.0)

get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})

set(DISPLAY_RT_DIR ${CMAKE_SOURCE_DIR})
message(STATUS "Entering: " ${DISPLAY_RT_DIR})

# project(sciq_util)
message(STATUS "Build project: " ${PROJECT_NAME})
message(STATUS "If cross-compiling: " ${CMAKE_CROSSCOMPILING})
message(STATUS "Target platform: " ${CMAKE_SYSTEM_NAME})
message(STATUS "Target system root: " ${CMAKE_SYSROOT})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

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

# # Required by SCIQ
# # LCM
# find_package(LCM REQUIRED)

# # Eigen3
# find_package(Eigen3 REQUIRED)
# set(EIGEN_INCLUDE_DIR "${EIGEN3_INCLUDE_DIR}")
# message(STATUS "Find Eigen3: " ${EIGEN_INCLUDE_DIR})

# # SCIQ
# set(SCIQ_DIR ${SCIQ_UTIL_DIR}/../sciq)

# set(LIB_SCIQ2_MATH_UTIL_HEADERS 
#     "${SCIQ_DIR}/include/math/algebra.h"
#     "${SCIQ_DIR}/include/math/geometry.h"
#     "${SCIQ_DIR}/include/math/kinematics.h"
#     "${SCIQ_DIR}/include/math/filter.h"
#     "${SCIQ_DIR}/include/math/timer.h"
# )

# set(LIB_SCIQ2_MATH_UTIL_SOURCES 
#     # "${SCIQ_DIR}/include/math/algebra.cc"
#     # "${SCIQ_DIR}/include/math/geometry.cc"
#     "${SCIQ_DIR}/include/math/kinematics.cc"
#     "${SCIQ_DIR}/include/math/filter.cc"
#     "${SCIQ_DIR}/include/math/timer.cc"
# )

# set(LIB_SCIQ2_CORE_UTIL_HEADERS 
#     # robot core
#     "${SCIQ_DIR}/include/core/abstraction_robot.h"
#     "${SCIQ_DIR}/include/core/abstraction_controller.h"
# )

# set(LIB_SCIQ2_CORE_UTIL_SOURCES 
#     # robot core
#     "${SCIQ_DIR}/include/core/abstraction_robot.cc"
#     # "${SCIQ_DIR}/include/core/abstraction_controller.cc"
# )

# set(LIB_SCIQ2_PARSER_UTIL_HEADERS 
#     # parser
#     "${SCIQ_DIR}/include/parser/abstraction_parser.h"
#     "${SCIQ_DIR}/include/parser/sciq2_monitor_parser_qcp.h"
#     "${SCIQ_DIR}/include/parser/sciq2_comm_parser_comm0.h"
# )

# set(LIB_SCIQ2_PARSER_UTIL_SOURCES 
#     # parser
#     "${SCIQ_DIR}/include/parser/sciq2_monitor_parser_qcp.cc"
#     "${SCIQ_DIR}/include/parser/sciq2_comm_parser_comm0.cc"
# )

# set(LIB_SCIQ2_COMM_UTIL_HEADERS 
#     # comm basics
#     "${SCIQ_DIR}/include/comm/abstraction_comm.h"
#     # messager
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_servos.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_sensors.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_bodies.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_legs.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_supervisor.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_composite_centroid.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_gait_scheduler.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_low_level_estimator.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_whole_body_estimator.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_whole_body_planner.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_swing_leg_planner.h"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_stance_leg_planner.h"
#     # # broadcaster
#     # "${SCIQ_DIR}/include/comm/broadcaster/sciq2_rigid_broadcaster0.h"
#     # # logger
#     # "${SCIQ_DIR}/include/comm/logger/sciq2_rigid_logger0.h"
# )

# set(LIB_SCIQ2_COMM_UTIL_SOURCES 
#     # comm basics
#     # "${SCIQ_DIR}/include/comm/abstraction_comm.h"
#     # messager
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_servos.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_sensors.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_bodies.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_legs.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_supervisor.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_composite_centroid.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_gait_scheduler.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_low_level_estimator.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_whole_body_estimator.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_whole_body_planner.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_swing_leg_planner.cc"
#     "${SCIQ_DIR}/include/comm/msg/sciq2_msg_stance_leg_planner.cc"
#     # # broadcaster
#     # "${SCIQ_DIR}/include/comm/broadcaster/sciq2_rigid_broadcaster0.cc"
#     # # logger
#     # "${SCIQ_DIR}/include/comm/logger/sciq2_rigid_logger0.cc"
# )

# set(LIB_SCIQ2_LCM_HEADERS
#     # robot
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/revolute_joint_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/prismatic_joint_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/joints_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/revolute_servo_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/prismatic_servo_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/servos_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/imu_sensor_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/contact_sensor_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/sensors_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/node_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/kinematics_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/bodies_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/legs_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/robot/physics_t.hpp"
#     # controller 
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/foothold_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/footholds_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/point_foot_states_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/multiple_point_foot_states_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/vector_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/rigid_body_states_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/composite_centroid/composite_centroid_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/gait_scheduler/gait_scheduler_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/supervisor/supervisor_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/low_level_estimator/low_level_estimator_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/whole_body_estimator/whole_body_estimator_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/whole_body_estimator/whole_body_state_mean_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/whole_body_estimator/whole_body_state_covariance_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/whole_body_planner/whole_body_planner_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/swing_leg_planner/swing_leg_planner_t.hpp"
#     "${SCIQ_DIR}/include/sciq2_lcm/controller/stance_leg_planner/stance_leg_planner_t.hpp"
# )

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
    # ${LIB_SCIQ2_MATH_UTIL_HEADERS} ${LIB_SCIQ2_MATH_UTIL_SOURCES}
    # ${LIB_SCIQ2_CORE_UTIL_HEADERS} ${LIB_SCIQ2_CORE_UTIL_SOURCES}
    # ${LIB_SCIQ2_PARSER_UTIL_HEADERS} ${LIB_SCIQ2_PARSER_UTIL_SOURCES}
    # ${LIB_SCIQ2_COMM_UTIL_HEADERS} ${LIB_SCIQ2_COMM_UTIL_SOURCES}
    # ${LIB_SCIQ2_LCM_HEADERS}
    ${LIB_DISPLAYRT_HEADERS} ${LIB_DISPLAYRT_SOURCES} 
    ${LIB_MINIYAML_HEADERS} ${LIB_MINIYAML_SOURCES}
)

target_link_libraries(displayrt PUBLIC 
    Qt5::Widgets Qt5::Core Qt5::PrintSupport Qt5::Gui 
    ${QCustomPlot_LIBRARY}
    ${LCM_LIBRARY}
    # ${qpOASES_LIBRARY}
)

target_include_directories(displayrt PUBLIC 
    # ${SCIQ_DIR}/include 
    ${DISPLAY_RT_DIR}/include 
    ${DISPLAY_RT_DIR}/thirdparty/mini-yaml/yaml 
    # ${EIGEN_INCLUDE_DIR} 
    # ${TINYFSM_INCLUDE_DIR} 
    # ${LCM_INCLUDE_DIR} 
    # ${qpOASES_INCLUDE_DIR} 
    ${QCustomPlot_INCLUDE_DIR} 
) 

set_target_properties(displayrt
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/lib"
)

# add_library(sciq2util SHARED 
#     ${LIB_SCIQ2_MATH_UTIL_HEADERS} ${LIB_SCIQ2_MATH_UTIL_SOURCES}
#     ${LIB_SCIQ2_CORE_UTIL_HEADERS} ${LIB_SCIQ2_CORE_UTIL_SOURCES}
#     ${LIB_SCIQ2_PARSER_UTIL_HEADERS} ${LIB_SCIQ2_PARSER_UTIL_SOURCES}
#     ${LIB_SCIQ2_COMM_UTIL_HEADERS} ${LIB_SCIQ2_COMM_UTIL_SOURCES}
#     ${LIB_SCIQ2_LCM_HEADERS}
#     ${LIB_UTIL_HEADERS} ${LIB_UTIL_SOURCES} 
#     ${LIB_MINIYAML_HEADERS} ${LIB_MINIYAML_SOURCES}
# )

# target_link_libraries(sciq2util PUBLIC 
#     Qt5::Widgets Qt5::Core Qt5::PrintSupport Qt5::Gui 
#     ${QCustomPlot_LIBRARY}
#     ${LCM_LIBRARY}
#     # ${qpOASES_LIBRARY}
# )

# target_include_directories(sciq2util PUBLIC 
#     ${SCIQ_DIR}/include 
#     ${SCIQ_UTIL_DIR}/include 
#     ${EIGEN_INCLUDE_DIR} 
#     # ${TINYFSM_INCLUDE_DIR} 
#     ${LCM_INCLUDE_DIR} 
#     # ${qpOASES_INCLUDE_DIR} 
#     ${QCustomPlot_INCLUDE_DIR} 
# ) 

# set_target_properties(sciq2util
#     PROPERTIES
#     LIBRARY_OUTPUT_DIRECTORY "${SCIQ_UTIL_DIR}/lib"
# )


# Add subdirectories
# add_subdirectory(test)
add_subdirectory(example)
# add_subdirectory(src)