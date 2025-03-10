# This is content of FindLCM.cmake
# message(STATUS "SCIQ_TOP_SOURCE_DIR=" ${SCIQ_TOP_SOURCE_DIR})

find_path(LCM_INCLUDE_DIR
          NAMES # all header files 
          lcm/lcm-cpp.hpp
          lcm/lcm-cpp-impl.hpp
          lcm/lcm.h
          lcm/lcm_coretypes.h
          lcm/lcm_export.h
          lcm/lcm_version.h
          lcm/eventlog.h
          PATHS # possible paths to search the header files
          /usr/local/include)

find_library(LCM_LIBRARY
             NAMES lcm # lib object, like .a, .so
             PATHS # possible paths to search the lib object
             /usr/local/lib)

if(LCM_INCLUDE_DIR AND LCM_LIBRARY)
    set(LCM_FOUND TRUE)
endif(LCM_INCLUDE_DIR AND LCM_LIBRARY)

if(LCM_FOUND)
    if(NOT LCM_FIND_QUIETLY)
        message(STATUS "Found LCM: " ${LCM_LIBRARY})
    endif(NOT LCM_FIND_QUIETLY)
else(LCM_FOUND)
    if(LCM_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find LCM library")
    endif(LCM_FIND_REQUIRED)
endif(LCM_FOUND)