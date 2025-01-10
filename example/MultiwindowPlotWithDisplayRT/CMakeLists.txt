message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MultiwindowPlotWithDisplayRT 
    MultiwindowPlotWithDisplayRT.cc
)

target_link_libraries(MultiwindowPlotWithDisplayRT 
    displayrt
)

set_target_properties(MultiwindowPlotWithDisplayRT
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

