# This is content of FindDislayRT.cmake

find_path(DisplayRT_INCLUDE_DIR
          NAMES # all header files 
            displayRT.h
          PATHS # possible paths to search the header files
            ${CMAKE_SOURCE_DIR}/include 
            ${CMAKE_SOURCE_DIR}/../include 
            ${CMAKE_SOURCE_DIR}/../../include 
          )

find_library(DisplayRT_LIBRARY
             NAMES # lib object, like .a, .so
                displayrt 
             PATHS # possible paths to search the lib object
                ${CMAKE_SOURCE_DIR}/lib 
                ${CMAKE_SOURCE_DIR}/../lib 
                ${CMAKE_SOURCE_DIR}/../../lib 
             )

if(DisplayRT_INCLUDE_DIR AND DisplayRT_LIBRARY)
    set(DisplayRT_FOUND TRUE)
endif(DisplayRT_INCLUDE_DIR AND DisplayRT_LIBRARY)

# if(DisplayRT_LIBRARY)
#     set(DisplayRT_FOUND TRUE)
# endif(DisplayRT_LIBRARY)

if(DisplayRT_FOUND)
    if(NOT DisplayRT_FIND_QUIETLY)
        message(STATUS "Found DisplayRT: " ${DisplayRT_LIBRARY})
    endif(NOT DisplayRT_FIND_QUIETLY)
else(DisplayRT_FOUND)
    if(DisplayRT_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find DisplayRT library")
    endif(DisplayRT_FIND_REQUIRED)
endif(DisplayRT_FOUND)