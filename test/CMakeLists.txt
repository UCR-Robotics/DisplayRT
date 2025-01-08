message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

# test_qcp
add_executable(test_qcp test_qcp.cc)

target_link_libraries(test_qcp displayrt)

# target_include_directories(test_qcp PUBLIC 
#     ${QCustomPlot_INCLUDE_DIR}
# )

set_target_properties(test_qcp
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

# test_rt_monitor
add_executable(test_display_rt 
                test_display_rt.cc
                # ../include/rt_monitor.h
                # ../include/rt_monitor.cc
)

target_link_libraries(test_display_rt displayrt)

# target_include_directories(test_rt_monitor PUBLIC 
#     ${QCustomPlot_INCLUDE_DIR}
#     ../include
# )

set_target_properties(test_display_rt
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

# # test_sciq2_monitor
# add_executable(test_sciq2_monitor 
#                 test_sciq2_monitor.cc
# )

# target_link_libraries(test_sciq2_monitor sciq2util)

# set_target_properties(test_sciq2_monitor
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${SCIQ_UTIL_DIR}/bin"
# )

# # test_sciq2_monitor_lcm
# add_executable(test_sciq2_monitor_lcm 
#                 test_sciq2_monitor_lcm.cc
# )

# target_link_libraries(test_sciq2_monitor_lcm sciq2util)

# set_target_properties(test_sciq2_monitor_lcm
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${SCIQ_UTIL_DIR}/bin"
# )

# # test_qcp
# add_executable(test_qcp test_qcp.cc)

# target_link_libraries(test_qcp Qt5::Widgets Qt5::Core Qt5::PrintSupport Qt5::Gui ${QCustomPlot_LIBRARY})

# target_include_directories(test_qcp PUBLIC 
#     ${QCustomPlot_INCLUDE_DIR}
# )

# set_target_properties(test_qcp
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${SCIQ_UTIL_DIR}/bin"
# )

# # test_rt_monitor
# add_executable(test_rt_monitor 
#                 test_rt_monitor.cc
#                 # ../include/rt_monitor.h
#                 ../include/rt_monitor.cc
# )

# target_link_libraries(test_rt_monitor Qt5::Widgets Qt5::Core Qt5::PrintSupport Qt5::Gui ${QCustomPlot_LIBRARY})

# target_include_directories(test_rt_monitor PUBLIC 
#     ${QCustomPlot_INCLUDE_DIR}
#     ../include
# )

# set_target_properties(test_rt_monitor
#     PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${SCIQ_UTIL_DIR}/bin"
# )