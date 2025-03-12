#include <memory>
#include <iostream>
#include <filesystem>

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

/***************/
/* LCM Message */
/***************/

struct Servos_Message
{
    std::string channel = "servos";

    msg::servos_t generate( const int64_t &timer )
    {
        // servos
        msg::servos_t data;
        data.timestamp = timer;
        data.revolute_servo_count = 2; 
        data.revolute_servos.resize(data.revolute_servo_count);
        
        // lambda: generate sinonoidal signal based on time
        auto generate_sin = [](const int64_t &timer, const double &amplitude, const double &frequency, const double &phase) -> double
        {
            double timer_s = timer / 1000.0;
            return amplitude * sin(2 * M_PI * frequency * timer + phase);
        };

        // lambda: generate time derivative of sinonoidal signal based on time
        auto generate_sin_dot = [](const int64_t &timer, const double &amplitude, const double &frequency, const double &phase) -> double
        {
            double timer_s = timer / 1000.0;
            return amplitude * 2 * M_PI * frequency * cos(2 * M_PI * frequency * timer + phase);
        };

        // lambda: generate white noise signal 
        auto generate_white_noise = [](const double &amplitude) -> double
        {
            return amplitude * (rand() % 1000) / 1000.0;
        };
        

      for(int i = 0; i < data.revolute_servo_count; i++)
      {
        data.revolute_servos[i].id = i;

        data.revolute_servos[i].commanded_position = generate_sin(timer, 1.0, 0.5, static_cast<double>(i) * 0.5);
        data.revolute_servos[i].commanded_velocity = generate_sin_dot(timer, 1.0, 0.5, static_cast<double>(i) * 0.5);
        data.revolute_servos[i].commanded_torque = 0.5 * data.revolute_servos[i].estimated_position; 

        data.revolute_servos[i].estimated_position = data.revolute_servos[i].commanded_position + generate_white_noise(0.1);
        data.revolute_servos[i].estimated_velocity = data.revolute_servos[i].commanded_velocity + generate_white_noise(0.1);
        data.revolute_servos[i].estimated_torque = data.revolute_servos[i].commanded_torque + generate_white_noise(0.1);
      }

      { // debug
        std::stringstream ss;
        ss << "Robot Servos Message : \n" 
           << "channel: " << channel << ","
           << "timestamp: " << data.timestamp << std::endl;
        
        for(int i = 0; i < data.revolute_servo_count; ++i)
        {
          ss << "revolute servo[" << i << "]: "
             << "id: " << data.revolute_servos[i].id << ","
             << "estimated position: " << data.revolute_servos[i].estimated_position << ","
             << "velocity: " << data.revolute_servos[i].estimated_velocity << ","
             << "torque: " << data.revolute_servos[i].estimated_torque << "; "
             << "commanded position: " << data.revolute_servos[i].commanded_position << ","
             << "velocity: " << data.revolute_servos[i].commanded_velocity << ","
             << "torque: " << data.revolute_servos[i].commanded_torque 
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

    msg::sensors_t generate( const int64_t &timer )
    {
        // sensors
        msg::sensors_t data;
        data.timestamp = timer;
        data.imu_sensor_count = 1; 
        data.imu_sensors.resize(data.imu_sensor_count);
        
        // lambda: generate sinonoidal signal based on time
        auto generate_sin = [](const int64_t &timer, const double &amplitude, const double &frequency, const double &phase) -> double
        {
            double timer_s = timer / 1000.0;
            return amplitude * sin(2 * M_PI * frequency * timer + phase);
        };

        // lambda: generate time derivative of sinonoidal signal based on time
        auto generate_sin_dot = [](const int64_t &timer, const double &amplitude, const double &frequency, const double &phase) -> double
        {
            double timer_s = timer / 1000.0;
            return amplitude * 2 * M_PI * frequency * cos(2 * M_PI * frequency * timer + phase);
        };

        // lambda: generate white noise signal 
        auto generate_white_noise = [](const double &amplitude) -> double
        {
            return amplitude * (rand() % 1000) / 1000.0;
        };
        

        for(int i = 0; i < data.imu_sensor_count; ++i)
        {
          data.imu_sensors[i].id = i;
          data.imu_sensors[i].quaternion[0] = 1.0;
          data.imu_sensors[i].quaternion[1] = 0.0;
          data.imu_sensors[i].quaternion[2] = 0.0;
          data.imu_sensors[i].quaternion[3] = 0.0;
          data.imu_sensors[i].euler_rate[0] = 0.1;
          data.imu_sensors[i].euler_rate[1] = 0.1;
          data.imu_sensors[i].euler_rate[2] = 0.1;
          data.imu_sensors[i].acceleration[0] = 0.2;
          data.imu_sensors[i].acceleration[1] = 0.2;
          data.imu_sensors[i].acceleration[2] = 0.2;
        }

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
            lcm_ = std::make_shared<lcm::LCM>("udpm://239.255.76.67:7667?ttl=2");
            if (!lcm_->good())
            {
                throw std::runtime_error("LCM is not good");
            }
            else
            {
                std::cout << "LCM is good" << std::endl;
            }
        }
    
    private:
        std::shared_ptr<lcm::LCM> lcm_;
        size_t count_revolute_servo_; 
        size_t count_imu_sensor_;
}; 


/*****************/
/* Display Class */
/*****************/

class myDisplayRT : public DisplayRT
{
public: 
    myDisplayRT( const std::shared_ptr<DisplayRT_Property> monitor_property )
    : DisplayRT(monitor_property)
    {
    }

    Status Setup() override
    {
        DisplayRT::defaultSetupDisplay();
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
    
};

