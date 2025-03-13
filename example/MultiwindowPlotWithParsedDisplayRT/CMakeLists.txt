message(STATUS "Entering: " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MultiwindowPlotWithParsedDisplayRT 
    MultiwindowPlotWithParsedDisplayRT.cc
    displayRT_parser.h 
)

target_link_libraries(MultiwindowPlotWithParsedDisplayRT 
    displayrt 
    # ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(MultiwindowPlotWithParsedDisplayRT
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

set_target_properties(MultiwindowPlotWithParsedDisplayRT
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DISPLAY_RT_DIR}/bin"
)

# Optional: LCM example
if( LCM_EXAMPLE )
    add_subdirectory(lcm)
endif()