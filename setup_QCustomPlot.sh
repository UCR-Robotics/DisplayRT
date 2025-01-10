#!/bin/bash

# Safe the current path for later use
current_path=$(pwd)

# Check if the /thirdparty/QCustomPlot/ directory exists in the current path. If not, exit with warning. If so, enter the directory.
if [ ! -d "$(pwd)/thirdparty/QCustomPlot" ]; then
    echo -e "\e[31mThe /thirdparty/QCustomPlot/ directory does not exist in the current path. Please make sure you are in the correct directory.\e[0m"
    exit 1
else
    cd "$(pwd)/thirdparty/QCustomPlot"
    echo -e "\e[32mEntered $(pwd).\e[0m"
fi

# # copy the qcustomplot.cpp.skeleton and qcustomplot.h.skeleton files 
# #       to qcustomplot.cpp and qcustomplot.h
# cp src/qcustomplot.cpp.skeleton qcustomplot.cpp
# cp src/qcustomplot.h.skeleton qcustomplot.h

# # check if sharedlib/sharedlib-compilation/sharedlib-compilation.pro exist 
# # and copy it to sharedlib-compilation.pro
# if [ ! -f "$(pwd)/sharedlib/sharedlib-compilation/sharedlib-compilation.pro" ]; then
#     echo -e "\e[31mThe sharedlib/sharedlib-compilation/sharedlib-compilation.pro file does not exist in the current path. Please make sure you are in the correct directory.\e[0m"
#     exit 1
# else
#     cp sharedlib/sharedlib-compilation/sharedlib-compilation.pro ./sharedlib-compilation.pro
#     echo -e "\e[32msharedlib/sharedlib-compilation/sharedlib-compilation.pro copied to sharedlib-compilation.pro.\e[0m"
# fi

# # modify the sharedlib-compilation.pro file:
# # replace the last two lines "SOURCES += ../../qcustomplot.cpp" and "HEADERS += ../../qcustomplot.h"
# #                       with "SOURCES += qcustomplot.cpp" and "HEADERS += qcustomplot.h"
# # append the target output directory "DESTDIR = $$PWD/lib"
# sed -i 's|SOURCES += ../../qcustomplot.cpp|SOURCES += qcustomplot.cpp|' sharedlib-compilation.pro
# sed -i 's|HEADERS += ../../qcustomplot.h|HEADERS += qcustomplot.h|' sharedlib-compilation.pro
# echo 'DESTDIR = $$PWD/lib' >> sharedlib-compilation.pro

# Check if the /build directory exists in the current path
if [ ! -d "$(pwd)/build" ]; then
    # If not, create the /build directory
    mkdir "$(pwd)/build"
    echo -e "\e[32m/build directory created.\e[0m"
else
    echo -e "\e[32m/build directory already exists.\e[0m"
fi

# Enter the /build directory
cd "$(pwd)/build"
echo -e "\e[32mEntered $(pwd).\e[0m"

# Run qmake and make the project
qmake ..
make

# # remove sharedlib-compilation.pro
# rm ../sharedlib-compilation.pro
# # remove .so with version number
# rm -f ../lib/libqcustomplot.so.*
# rm -f ../lib/libqcustomplotd.so.*

# Return to the original path
cd $current_path
echo -e "\e[32mReturned to $current_path.\e[0m"

# Exit the script
exit 0
