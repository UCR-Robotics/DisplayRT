message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

find_package(LCM REQUIRED)

set( LCM_EXAMPLE_SOURCE )

set( LCM_EXAMPLE_HEADER 
    ../displayRT_parser.h 
    myDisplayWithLCM.h 
    msg/imu_sensor_t.hpp 
    msg/revolute_servo_t.hpp 
    msg/sensors_t.hpp 
    msg/servos_t.hpp 
)

add_executable(MultiwindowPlotWithParsedDisplayRTandLCM 
    MultiwindowPlotWithParsedDisplayRTandLCM.cc 
    ${LCM_EXAMPLE_SOURCE} 
    ${LCM_EXAMPLE_HEADER} 
)

target_link_libraries(MultiwindowPlotWithParsedDisplayRTandLCM 
    displayrt 
    # ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(MultiwindowPlotWithParsedDisplayRTandLCM 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/.. 
)

set_target_properties(MultiwindowPlotWithParsedDisplayRTandLCM
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

