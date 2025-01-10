message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MultiwindowPlotWithQCustomPlot 
    MultiwindowPlotWithQCustomPlot.cc
)

target_link_libraries(MultiwindowPlotWithQCustomPlot 
    displayrt
)

set_target_properties(MultiwindowPlotWithQCustomPlot
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

