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
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "displayrt_example_ros2/msg/imu.hpp"
#include "displayrt_example_ros2/msg/sensors.hpp"
#include "displayrt_example_ros2/msg/revolute_servo.hpp"
#include "displayrt_example_ros2/msg/servos.hpp"


using namespace display_rt; // for DisplayRT
using namespace std::chrono_literals;
using std::placeholders::_1;

namespace display_rt::example {

/******************/
/* ROS2 Publisher */
/******************/

class myPublisherROS2 : public rclcpp::Node
{
    public: 
        myPublisherROS2(); 

        int runOnce()
        {
            publishServos();
            publishSensors();
            return 0;
        }
        
    
    private:
        // std::shared_ptr<lcm::LCM> lcm_;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<displayrt_example_ros2::msg::Sensors>::SharedPtr publisher_sensors_;
        rclcpp::Publisher<displayrt_example_ros2::msg::Servos>::SharedPtr publisher_servos_;
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

class mySubscriberROS2 : public rclcpp::Node
{
    public: 
        mySubscriberROS2();

        int spinOnce()
        {
            rclcpp::spin_some(this->get_node_base_interface());
            return 0;
        }
    
    private:
        rclcpp::Subscription<displayrt_example_ros2::msg::Servos>::SharedPtr subscription_servos_;
        rclcpp::Subscription<displayrt_example_ros2::msg::Sensors>::SharedPtr subscription_sensors_;
        
        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<displayrt_example_ros2::msg::Servos> servos_data_;
        std::shared_ptr<displayrt_example_ros2::msg::Sensors> sensors_data_;
    
    private: // internal methods
        void servosCallbackHandler(const displayrt_example_ros2::msg::Servos::SharedPtr msg);
        void sensorsCallbackHandler(const displayrt_example_ros2::msg::Sensors::SharedPtr msg);
}; 


/*****************/
/* Display Class */
/*****************/

class myDisplayRT_ROS2 : public DisplayRT, public rclcpp::Node
{
    Q_OBJECT

    public: 
        myDisplayRT_ROS2( const std::shared_ptr<DisplayRT_Property> monitor_property ); 

        Status Setup() override;   

        Status Update() override; 

    private: 
        // ROS2 
        rclcpp::Subscription<displayrt_example_ros2::msg::Servos>::SharedPtr subscription_servos_;
        rclcpp::Subscription<displayrt_example_ros2::msg::Sensors>::SharedPtr subscription_sensors_;

        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<displayrt_example_ros2::msg::Servos> servos_data_;
        std::shared_ptr<displayrt_example_ros2::msg::Sensors> sensors_data_;

    private: // internal methods
        #ifdef ROS2_FOXY
        void servosCallbackHandler(const displayrt_example_ros2::msg::Servos::SharedPtr msg);
        #elif defined(ROS2_GALACTIC) || defined(ROS2_HUMBLE) || defined(ROS2_JAZZY)
        void servosCallbackHandler(const displayrt_example_ros2::msg::Servos &msg);
        #endif 

        #ifdef ROS2_FOXY
        void sensorsCallbackHandler(const displayrt_example_ros2::msg::Sensors::SharedPtr msg);
        #elif defined(ROS2_GALACTIC) || defined(ROS2_HUMBLE) || defined(ROS2_JAZZY)
        void sensorsCallbackHandler(const displayrt_example_ros2::msg::Sensors &msg);
        #endif
};

}// namespace display_rt::example

