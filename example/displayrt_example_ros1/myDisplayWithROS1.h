#include <memory>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cmath>
#include <thread>
#include <cstdlib>
#include <string>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>

// #include "qcustomplot.h"
#include "displayRT.h"
#include "displayRT_parser.h"

// #include <lcm/lcm-cpp.hpp>
// #include "msg/servos_t.hpp"
// #include "msg/sensors_t.hpp"

// ROS2
#include "ros/ros.h"
#include "displayrt_example_ros1/Sensors.h"
#include "displayrt_example_ros1/Servos.h"
#include "displayrt_example_ros1/RevoluteServo.h"
#include "displayrt_example_ros1/IMU.h"

using namespace display_rt; // for DisplayRT
using namespace std::chrono_literals;
using std::placeholders::_1;

namespace display_rt::example {

/******************/
/* ROS2 Publisher */
/******************/

class myPublisherROS1
{
    public: 
        myPublisherROS1(); 

        int runOnce()
        {
            publishServos();
            publishSensors();
            return 0;
        }
        
    
    private:
        std::shared_ptr< ros::NodeHandle > node_handle_;
        std::shared_ptr< ros::Publisher > publisher_sensors_;
        std::shared_ptr< ros::Publisher > publisher_servos_;
        
        // size_t count_;

        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";
        size_t count_revolute_servo_ = 2; 
        size_t count_imu_sensor_ = 1;


        // timer
        std::chrono::steady_clock::time_point start_time_; // Start time

    private: // internal methods
        const long long getElapsedTimeInMilliseconds()
        {
            const auto current_time = std::chrono::steady_clock::now();
            const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - this->start_time_);
            return elapsed_time.count();
        }

        void publishServos(); 

        void publishSensors(); 

        const float generateSin(const long long timer, const float amplitude, const float frequency, const float phase)
        {
            float timer_s = static_cast<float>(timer) / 1000.0f;
            return amplitude * std::sin(2.0f * M_PI * frequency * timer + phase);
        }

        const float generateSinDot(const long long timer, const float amplitude, const float frequency, const float phase)
        {
            float timer_s = static_cast<float>(timer) / 1000.0f;
            return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer + phase);
        }

        const float generateWhiteNoise(const float amplitude)
        {
            return amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);
        }

}; 

/*******************/
/* ROS2 Subscriber */
/*******************/

class mySubscriberROS1
{
    public: 
        mySubscriberROS1();

        int spinOnce()
        {
            ros::spinOnce();
            return 0;
        }
    
    private:
        // ROS1
        std::shared_ptr<ros::NodeHandle> node_handle_;
        std::shared_ptr<ros::Subscriber> subscriber_servos_;
        std::shared_ptr<ros::Subscriber> subscriber_sensors_;        
        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<displayrt_example_ros1::Servos> servos_data_;
        std::shared_ptr<displayrt_example_ros1::Sensors> sensors_data_;
    
    private: // internal methods
        void servosCallbackHandler(const displayrt_example_ros1::Servos::ConstPtr &msg);
        void sensorsCallbackHandler(const displayrt_example_ros1::Sensors::ConstPtr &msg);
}; 


/*****************/
/* Display Class */
/*****************/

class myDisplayRT_ROS2 : public DisplayRT
{
    Q_OBJECT

    public: 
        myDisplayRT_ROS2( const std::shared_ptr<DisplayRT_Property> monitor_property ); 

        Status Setup() override;   

        Status Update() override; 

    private: 
        // ROS1
        std::shared_ptr<ros::NodeHandle> node_handle_;
        std::shared_ptr<ros::Subscriber> subscriber_servos_;
        std::shared_ptr<ros::Subscriber> subscriber_sensors_;

        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<displayrt_example_ros1::Servos> servos_data_;
        std::shared_ptr<displayrt_example_ros1::Sensors> sensors_data_;

    private: // internal methods
    void servosCallbackHandler(const displayrt_example_ros1::Servos::ConstPtr &msg);
    void sensorsCallbackHandler(const displayrt_example_ros1::Sensors::ConstPtr &msg);
};

}// namespace display_rt::example

