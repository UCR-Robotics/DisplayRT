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

    std::shared_ptr< msg::servos_t > generate( const int64_t timer )
    {
        // servos
        auto data = std::make_shared<msg::servos_t>();
        data->timestamp = timer;
        data->revolute_servo_count = this->revolute_servo_count; 
        data->revolute_servos.resize(data->revolute_servo_count);
        
        // lambda: generate sinonoidal signal based on time
        auto generate_sin = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
        {
            float timer_s = static_cast<float>(timer) / 1000.0f;
            return amplitude * std::sin(2.0f * M_PI * frequency * timer + phase);
        };

        // lambda: generate time derivative of sinonoidal signal based on time
        auto generate_sin_dot = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
        {
            float timer_s = timer / 1000.0f;
            return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer + phase);
        };

        // lambda: generate white noise signal 
        auto generate_white_noise = [](const float amplitude) -> float
        {
            return amplitude * static_cast<float>(rand() % 1000) / 1000.0f;
        };

        for(int i = 0; i < static_cast<int>( data->revolute_servo_count ); i++)
        {
            data->revolute_servos[i].id = i;

            data->revolute_servos[i].commanded_position = generate_sin(timer, 1.0, 0.5, static_cast<double>(i) * 0.5);
            data->revolute_servos[i].commanded_velocity = generate_sin_dot(timer, 1.0, 0.5, static_cast<double>(i) * 0.5);
            data->revolute_servos[i].commanded_torque = 0.5 * data->revolute_servos[i].commanded_position; 

            data->revolute_servos[i].estimated_position = data->revolute_servos[i].commanded_position + generate_white_noise(0.1);
            data->revolute_servos[i].estimated_velocity = data->revolute_servos[i].commanded_velocity + generate_white_noise(0.1);
            data->revolute_servos[i].estimated_torque = data->revolute_servos[i].commanded_torque + generate_white_noise(0.1);
        }

        { // debug
            std::stringstream ss;
            ss << "Publishing Servos Message : \n" 
            << "channel: " << channel << ","
            << "timestamp: " << data->timestamp << std::endl;
            
            for(int i = 0; i < static_cast<int>( data->revolute_servo_count); ++i)
            {
            ss << "revolute servo[" << i << "]: "
                << "id: " << static_cast<int>( data->revolute_servos[i].id ) << ","
                << "estimated position: " << data->revolute_servos[i].estimated_position << ","
                << "velocity: " << data->revolute_servos[i].estimated_velocity << ","
                << "torque: " << data->revolute_servos[i].estimated_torque << "; "
                << "commanded position: " << data->revolute_servos[i].commanded_position << ","
                << "velocity: " << data->revolute_servos[i].commanded_velocity << ","
                << "torque: " << data->revolute_servos[i].commanded_torque 
                << std::endl;
            }
            
            std::cout << ss.str() << std::endl;
        }

        return data;
    }
};

struct Sensors_Message
{
    std::string channel = "sensors";
    int imu_sensor_count = 1;

    Sensors_Message( const std::string &channel = "sensors", const int &imu_sensor_count = 1 )
    {
        this->channel = channel;
        this->imu_sensor_count = imu_sensor_count;
    }

    std::shared_ptr< msg::sensors_t > generate( const int64_t timer )
    {
        // sensors
        auto data = std::make_shared<msg::sensors_t>();
        data->timestamp = timer;
        data->imu_sensor_count = this->imu_sensor_count; 
        data->imu_sensors.resize(data->imu_sensor_count);
        
        // lambda: generate sinonoidal signal based on time
        auto generate_sin = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
        {
            float timer_s = static_cast<float>( timer ) / 1000.0f;
            return amplitude * std::sin(2.0f * M_PI * frequency * timer + phase);
        };

        // lambda: generate time derivative of sinonoidal signal based on time
        auto generate_sin_dot = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
        {
            float timer_s = static_cast<float>( timer ) / 1000.0f;
            return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer + phase);
        };

        // lambda: generate white noise signal 
        auto generate_white_noise = [](const float amplitude) -> float
        {
            return amplitude * static_cast<float>(rand() % 1000) / 1000.0f;
        };    

        for(int i = 0; i < static_cast<int>( data->imu_sensor_count ); ++i)
        {
          data->imu_sensors[i].id = i;
          data->imu_sensors[i].quaternion[0] = 1.0;
          data->imu_sensors[i].quaternion[1] = 0.0;
          data->imu_sensors[i].quaternion[2] = 0.0;
          data->imu_sensors[i].quaternion[3] = 0.0;
          data->imu_sensors[i].euler_rate[0] = generate_sin(timer, 0.1, 0.5, static_cast<double>(i) * 0.5);
          data->imu_sensors[i].euler_rate[1] = generate_sin(timer, 0.1, 0.5, static_cast<double>(i) * 0.5);
          data->imu_sensors[i].euler_rate[2] = generate_sin(timer, 0.1, 0.5, static_cast<double>(i) * 0.5);
          data->imu_sensors[i].acceleration[0] = generate_sin(timer, 0.1, 1, static_cast<double>(i) * 0.5);
          data->imu_sensors[i].acceleration[1] = generate_sin(timer, 0.1, 1, static_cast<double>(i) * 0.5);
          data->imu_sensors[i].acceleration[2] = generate_sin(timer, 0.1, 1, static_cast<double>(i) * 0.5);
        }

        { // debug
            std::stringstream ss;
            ss << "Publishing Sensors Message : \n" 
                 << "channel: " << channel << ","
                 << "timestamp: " << data->timestamp << std::endl;
            
            for(int i = 0; i < static_cast<int>( data->imu_sensor_count ); ++i)
            {
                ss << "imu sensor[" << i << "]: "
                 << "id: " << static_cast<int>( data->imu_sensors[i].id ) << ","
                 << "quaternion: " << data->imu_sensors[i].quaternion[0] << ", " << data->imu_sensors[i].quaternion[1] << ", " << data->imu_sensors[i].quaternion[2] << ", " << data->imu_sensors[i].quaternion[3] << ";"
                 << "euler rate: " << data->imu_sensors[i].euler_rate[0] << ", " << data->imu_sensors[i].euler_rate[1] << ", " << data->imu_sensors[i].euler_rate[2] << ";"
                 << "acceleration: " << data->imu_sensors[i].acceleration[0] << ", " << data->imu_sensors[i].acceleration[1] << ", " << data->imu_sensors[i].acceleration[2] 
                 << std::endl;
            }
            
            std::cout << ss.str() << std::endl;
        }

        return data;
    }
};


/*****************/
/* LCM Publisher */
/*****************/

class myPublisherLCM
{
    public: 
        myPublisherLCM()
        {
            this->lcm_ = std::make_shared<lcm::LCM>("udpm://239.255.76.67:7667?ttl=2");
            if (!this->lcm_->good())
            {
                throw std::runtime_error("[WARNING] myPublisherLCM: LCM is not good");
            }
            else
            {
                std::cout << "[INFO] myPublisherLCM: LCM is good" << std::endl;
            }

            this->servos_message_ = std::make_shared<Servos_Message>( this->channel_servos_, this->count_revolute_servo_ );
            this->sensors_message_ = std::make_shared<Sensors_Message>(  this->channel_sensors_, this->count_imu_sensor_ );

            // Capture the start time
            start_time_ = std::chrono::steady_clock::now();
        }

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
        mySubscriberLCM()
        {
            this->lcm_ = std::make_shared<lcm::LCM>("udpm://239.255.76.67:7667?ttl=2");
            if (!lcm_->good())
            {
                throw std::runtime_error("[WARNING] mySubscriberLCM: LCM is not good");
            }
            else
            {
                std::cout << "[INFO] mySubscriberLCM: LCM is good" << std::endl;
            }

            lcm_->subscribe( this->channel_servos_, &mySubscriberLCM::servosCallbackHandler, this);
            lcm_->subscribe( this->channel_sensors_, &mySubscriberLCM::sensorsCallbackHandler, this);
        }

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
        void servosCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::servos_t* msg)
        {
            this->servos_data_ = std::make_shared<msg::servos_t>(*msg);
            
            { // debug
                std::stringstream ss;
                ss << "Receiving Servos Message : \n" 
                    << "channel: " << channel << ","
                    << "timestamp: " << this->servos_data_->timestamp << std::endl;
                
                for(int i = 0; i < static_cast<int>( this->servos_data_->revolute_servo_count ); ++i)
                {
                    ss << "revolute servo[" << i << "]: "
                        << "id: " << static_cast<int>( this->servos_data_->revolute_servos[i].id ) << ","
                        << "estimated position: " << this->servos_data_->revolute_servos[i].estimated_position << ","
                        << "velocity: " << this->servos_data_->revolute_servos[i].estimated_velocity << ","
                        << "torque: " << this->servos_data_->revolute_servos[i].estimated_torque << "; "
                        << "commanded position: " << this->servos_data_->revolute_servos[i].commanded_position << ","
                        << "velocity: " << this->servos_data_->revolute_servos[i].commanded_velocity << ","
                        << "torque: " << this->servos_data_->revolute_servos[i].commanded_torque 
                        << std::endl;
                }
                
                std::cout << ss.str() << std::endl;
            }
        }

        void sensorsCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::sensors_t* msg)
        {
            this->sensors_data_ = std::make_shared<msg::sensors_t>(*msg);
            
            { // debug
                std::stringstream ss;
                ss << "Receiving Sensors Message : \n" 
                    << "channel: " << channel << ","
                    << "timestamp: " << this->sensors_data_->timestamp << std::endl;
                
                for(int i = 0; i < static_cast<int>( this->sensors_data_->imu_sensor_count ); ++i)
                {
                    ss << "imu sensor[" << i << "]: "
                        << "id: " << static_cast<int>( this->sensors_data_->imu_sensors[i].id ) << ","
                        << "quaternion: " << this->sensors_data_->imu_sensors[i].quaternion[0] << ", " << this->sensors_data_->imu_sensors[i].quaternion[1] << ", " << this->sensors_data_->imu_sensors[i].quaternion[2] << ", " << this->sensors_data_->imu_sensors[i].quaternion[3] << ";"
                        << "euler rate: " << this->sensors_data_->imu_sensors[i].euler_rate[0] << ", " << this->sensors_data_->imu_sensors[i].euler_rate[1] << ", " << this->sensors_data_->imu_sensors[i].euler_rate[2] << ";"
                        << "acceleration: " << this->sensors_data_->imu_sensors[i].acceleration[0] << ", " << this->sensors_data_->imu_sensors[i].acceleration[1] << ", " << this->sensors_data_->imu_sensors[i].acceleration[2] 
                        << std::endl;
                }
                
                std::cout << ss.str() << std::endl;
            }
        }
}; 


/*****************/
/* Display Class */
/*****************/

class myDisplayRT : public DisplayRT
{
    Q_OBJECT

    public: 
        myDisplayRT( const std::shared_ptr<DisplayRT_Property> monitor_property )
        : DisplayRT(monitor_property)
        {
            this->lcm_ = std::make_shared<lcm::LCM>("udpm://239.255.76.67:7667?ttl=2");
            if (!lcm_->good())
            {
                throw std::runtime_error("[WARNING] mySubscriberLCM: LCM is not good");
            }
            else
            {
                std::cout << "[INFO] mySubscriberLCM: LCM is good" << std::endl;
            }

            lcm_->subscribe( this->channel_servos_, &myDisplayRT::servosCallbackHandler, this);
            lcm_->subscribe( this->channel_sensors_, &myDisplayRT::sensorsCallbackHandler, this);
        }

        Status Setup() override
        {
            // DisplayRT::defaultSetupDisplay();

            QObject::connect( this, &myDisplayRT::servosCallbackHandler, this, &myDisplayRT::Update );

            return Status::NORMAL;
        }    

        Status Update() override
        {
            DisplayRT::defaultUpdateDisplay();
            return Status::NORMAL;
        }

    private: 
        // LCM
        std::shared_ptr<lcm::LCM> lcm_;

        std::string channel_servos_ = "servos";
        std::string channel_sensors_ = "sensors";

        std::shared_ptr<msg::servos_t> servos_data_;
        std::shared_ptr<msg::sensors_t> sensors_data_;

    private: // internal methods
        void servosCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::servos_t* msg)
        {
            this->servos_data_ = std::make_shared<msg::servos_t>(*msg);
            
            { // debug
                std::stringstream ss;
                ss << "Receiving Servos Message : \n" 
                    << "channel: " << channel << ","
                    << "timestamp: " << this->servos_data_->timestamp << std::endl;
                
                for(int i = 0; i < static_cast<int>( this->servos_data_->revolute_servo_count ); ++i)
                {
                    ss << "revolute servo[" << i << "]: "
                        << "id: " << static_cast<int>( this->servos_data_->revolute_servos[i].id ) << ","
                        << "estimated position: " << this->servos_data_->revolute_servos[i].estimated_position << ","
                        << "velocity: " << this->servos_data_->revolute_servos[i].estimated_velocity << ","
                        << "torque: " << this->servos_data_->revolute_servos[i].estimated_torque << "; "
                        << "commanded position: " << this->servos_data_->revolute_servos[i].commanded_position << ","
                        << "velocity: " << this->servos_data_->revolute_servos[i].commanded_velocity << ","
                        << "torque: " << this->servos_data_->revolute_servos[i].commanded_torque 
                        << std::endl;
                }
                
                std::cout << ss.str() << std::endl;
            }
        }

        void sensorsCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::sensors_t* msg)
        {
            this->sensors_data_ = std::make_shared<msg::sensors_t>(*msg);
            
            { // debug
                std::stringstream ss;
                ss << "Receiving Sensors Message : \n" 
                    << "channel: " << channel << ","
                    << "timestamp: " << this->sensors_data_->timestamp << std::endl;
                
                for(int i = 0; i < static_cast<int>( this->sensors_data_->imu_sensor_count ); ++i)
                {
                    ss << "imu sensor[" << i << "]: "
                        << "id: " << static_cast<int>( this->sensors_data_->imu_sensors[i].id ) << ","
                        << "quaternion: " << this->sensors_data_->imu_sensors[i].quaternion[0] << ", " << this->sensors_data_->imu_sensors[i].quaternion[1] << ", " << this->sensors_data_->imu_sensors[i].quaternion[2] << ", " << this->sensors_data_->imu_sensors[i].quaternion[3] << ";"
                        << "euler rate: " << this->sensors_data_->imu_sensors[i].euler_rate[0] << ", " << this->sensors_data_->imu_sensors[i].euler_rate[1] << ", " << this->sensors_data_->imu_sensors[i].euler_rate[2] << ";"
                        << "acceleration: " << this->sensors_data_->imu_sensors[i].acceleration[0] << ", " << this->sensors_data_->imu_sensors[i].acceleration[1] << ", " << this->sensors_data_->imu_sensors[i].acceleration[2] 
                        << std::endl;
                }
                
                std::cout << ss.str() << std::endl;
            }
        }
    
};

}// namespace display_rt::example

