# DisplayRT
This is a utility program for displaying data stream in soft realtime in multiple windows. 
This project is originally motivated to visualize data of various types from a mobile robot. 
It is essentially an abstraction class named **DisplayRT** that can be coordinated directly in your project or with the middleware like ROS/ROS2 or LCM.  

# Dependencies

Ubuntu 20.04 LTS and 22.04 LTS

- **QT5**: the dependencies can be installed via the command ```sudo apt install qtbase5-dev qt5-qmake qtbase5-dev-tools libqt5widgets5```. 
- **QCustomPlot**: the source code (v2.1.0) is included in the folder */thirdparty/QCustomPlot*. Run setup_QCustomPlot.sh for library compilation. 
- **mini-yaml**: the submodule (main) is included in the folder */thirdparty/mini-plot*. 



# Compilation
The easiest way is to run the script *setup.sh* in the root folder. It will install QT5 dependencies and compile the project. 
You can also compile it as a normal CMake project. 

# Usage


# Reference

## Acknowledgments
This project includes the following third-party components:
- [QCustomPlot](git@gitlab.com:ecme2/QCustomPlot.git), licensed under the GPL License. See `thirdparty/QCustomPlot/GPL.txt` for details. Source code can be downloaded [here](https://www.qcustomplot.com/index.php/download). 
- [mini-yaml](https://github.com/jimmiebergmann/mini-yaml.git), licensed under the MIT License. See `thirdparty/mini-yaml/LICENSE` for details.