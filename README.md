# DisplayRT
This is a utility program for displaying data stream in soft realtime in multiple windows. 
This project is originally motivated to visualize data of various types for a mobile robot. 

<div style="text-align: left;">
  <img src="doc/DisplayRT_example.gif" alt="DisplayRT" width="800"/>
</div>

# Dependencies
Ubuntu 20.04 LTS, 22.04 LTS, and 24.04 LTS

- **QT5**: the dependencies can be installed via the command ```sudo apt install qtbase5-dev qt5-qmake qtbase5-dev-tools libqt5widgets5```. 
- **QCustomPlot**: the source code (v2.1.0) is included in the folder */thirdparty/QCustomPlot*. Run setup_QCustomPlot.sh for library compilation. 
- **mini-yaml**: the submodule (main) is included in the folder */thirdparty/mini-plot*. 

# Compilation
The easiest way is to run the script *setup.sh* in the root folder. It will install QT5 dependencies and compile the project. 
You can also manually compile it as a normal CMake project. 

# Usage
It is essentially an abstraction class named **DisplayRT** that can be coordinated directly in your project or with the middleware like ROS1/ROS2 or LCM. 
You should develop your display class by inheriting **DisplayRT** and implement the virtual methods *Setup()* and *Update()*. 
A quick start would be calling *defaultSetupDisplay()* and *defaultUpdateDisplay()* in your *Setup()* and *Update()* implementations respectively. 

The plotting options are consistent with QCustomPlot and summarized in [usage.md](./doc/usage.md). 
See examples in /example folder for more usage cases. 
[ROS1](example/displayrt_example_ros1/README.md), [ROS2](example/displayrt_example_ros2/README.md), [LCM](example/displayrt_example_lcm/README.md) examples are compiled separately as individual projects. 

# Acknowledgments
This project includes the following third-party components:
- [QCustomPlot](git@gitlab.com:ecme2/QCustomPlot.git), licensed under the GPL License. See `thirdparty/QCustomPlot/GPL.txt` for details. Source code can be downloaded [here](https://www.qcustomplot.com/index.php/download). 
- [mini-yaml](https://github.com/jimmiebergmann/mini-yaml.git), licensed under the MIT License. See `thirdparty/mini-yaml/LICENSE` for details.

## TODO
- add yaml-cpp support