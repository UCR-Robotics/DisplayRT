# This is content of FindQCustomPlot.cmake

find_path(QCustomPlot_INCLUDE_DIR
          NAMES # all header files 
            qcustomplot.h
          PATHS # possible paths to search the header files
            ${CMAKE_SOURCE_DIR}/thirdparty/QCustomPlot 
            ${CMAKE_SOURCE_DIR}/../thirdparty/QCustomPlot
            ${CMAKE_SOURCE_DIR}/../../thirdparty/QCustomPlot
          )

find_library(QCustomPlot_LIBRARY
             NAMES # lib object, like .a, .so
                qcustomplot 
             PATHS # possible paths to search the lib object
                ${CMAKE_SOURCE_DIR}/thirdparty/QCustomPlot/lib 
                ${CMAKE_SOURCE_DIR}/../thirdparty/QCustomPlot/lib
                ${CMAKE_SOURCE_DIR}/../../thirdparty/QCustomPlot/lib
             )

if(QCustomPlot_INCLUDE_DIR AND QCustomPlot_LIBRARY)
    set(QCustomPlot_FOUND TRUE)
endif(QCustomPlot_INCLUDE_DIR AND QCustomPlot_LIBRARY)

# if(QCustomPlot_LIBRARY)
#     set(QCustomPlot_FOUND TRUE)
# endif(QCustomPlot_LIBRARY)

if(QCustomPlot_FOUND)
    if(NOT QCustomPlot_FIND_QUIETLY)
        message(STATUS "Found QCustomPlot: " ${QCustomPlot_LIBRARY})
    endif(NOT QCustomPlot_FIND_QUIETLY)
else(QCustomPlot_FOUND)
    if(QCustomPlot_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find QCustomPlot library")
    endif(QCustomPlot_FIND_REQUIRED)
endif(QCustomPlot_FOUND)