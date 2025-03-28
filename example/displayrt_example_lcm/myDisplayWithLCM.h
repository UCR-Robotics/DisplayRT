#include <memory>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cmath>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>

// #include "qcustomplot.h"
#include "displayRT.h"
#include "displayRT_parser.h"

#include <lcm/lcm-cpp.hpp>
#include "msg/servos_t.hpp"
#include "msg/sensors_t.hpp"


using namespace display_rt; // for DisplayRT

namespace display_rt::example {

/*****************/
/* LCM Publisher */
/*****************/

class myPublisherLCM
{
    public: 
        myPublisherLCM(); 

        int runOnce()
        {
            publishServos();
            publishSensors();
            return 0;
        }
        
    
    private:
        std::shared_ptr<lcm::LCM> lcm_;

        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";
        size_t count_revolute_servo_ = 2; 
        size_t count_imu_sensor_ = 1;


        // std::shared_ptr<Servos_Message> servos_message_;
        // std::shared_ptr<Sensors_Message> sensors_message_;
        std::shared_ptr<msg::servos_t> servos_data_;
        std::shared_ptr<msg::sensors_t> sensors_data_;

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
        // {
        //     servos_data_ = servos_message_->generate( getElapsedTimeInMilliseconds() );
        //     lcm_->publish( servos_message_->channel, servos_data_.get() );
        // }

        void publishSensors(); 
        // {
        //     sensors_data_ = sensors_message_->generate( getElapsedTimeInMilliseconds() );
        //     lcm_->publish( sensors_message_->channel, sensors_data_.get() );
        // }

        const float generateSin(const long long timer, const float amplitude, const float frequency, const float phase)
        {
            float timer_s = static_cast<float>(timer) / 1000.0f;
            return amplitude * std::sin(2.0f * M_PI * frequency * timer_s + phase);
        }

        const float generateSinDot(const long long timer, const float amplitude, const float frequency, const float phase)
        {
            float timer_s = static_cast<float>(timer) / 1000.0f;
            return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer_s + phase);
        }

        const float generateWhiteNoise(const float amplitude)
        {
            return amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);
        }

}; 

/******************/
/* LCM Subscriber */
/******************/

class mySubscriberLCM
{
    public: 
        mySubscriberLCM(); 

        int spinOnce()
        {
            this->lcm_->handle();
            return 0;
        }
    
    private:
        std::shared_ptr<lcm::LCM> lcm_;
        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<msg::servos_t> servos_data_;
        std::shared_ptr<msg::sensors_t> sensors_data_;
    
    private: // internal methods
        void servosCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::servos_t* msg); 

        void sensorsCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::sensors_t* msg); 
}; 


/*****************/
/* Display Class */
/*****************/

class myDisplayRT : public DisplayRT
{
    Q_OBJECT

    public: 
        myDisplayRT( const std::shared_ptr<DisplayRT_Property> monitor_property ); 

        Status Setup() override;   

        Status Update() override; 

    private: 
        // LCM
        std::shared_ptr<lcm::LCM> lcm_;

        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<msg::servos_t> servos_data_;
        std::shared_ptr<msg::sensors_t> sensors_data_;

    private: // internal methods
        void servosCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::servos_t* msg); 

        void sensorsCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::sensors_t* msg); 
};

}// namespace display_rt::example

