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

/***************/
/* LCM Message */
/***************/

struct Servos_Message
{
    std::string channel = "servos";
    int revolute_servo_count = 2;

    Servos_Message( const std::string &channel = "servos", const int revolute_servo_count = 2 )
    {
        this->channel = channel;
        this->revolute_servo_count = revolute_servo_count;
    }

    std::shared_ptr< msg::servos_t > generate( const int64_t timer ); 
};

struct Sensors_Message
{
    std::string channel = "sensors";
    int imu_sensor_count = 1;

    Sensors_Message( const std::string &channel = "sensors", const int imu_sensor_count = 1 )
    {
        this->channel = channel;
        this->imu_sensor_count = imu_sensor_count;
    }

    std::shared_ptr< msg::sensors_t > generate( const int64_t timer ); 
};


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


        std::shared_ptr<Servos_Message> servos_message_;
        std::shared_ptr<Sensors_Message> sensors_message_;
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

        void publishServos()
        {
            servos_data_ = servos_message_->generate( getElapsedTimeInMilliseconds() );
            lcm_->publish( servos_message_->channel, servos_data_.get() );
        }

        void publishSensors()
        {
            sensors_data_ = sensors_message_->generate( getElapsedTimeInMilliseconds() );
            lcm_->publish( sensors_message_->channel, sensors_data_.get() );
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

