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

#include "myDisplayWithROS2Humble.h"


using namespace display_rt; // for DisplayRT

namespace display_rt::example {

/***************/
/* LCM Message */
/***************/

std::shared_ptr< msg::servos_t > Servos_Message::generate( const int64_t timer )
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
        return amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);
    };

    for(int i = 0; i < static_cast<int>( data->revolute_servo_count ); i++)
    {
        data->revolute_servos[i].id = i;

        data->revolute_servos[i].commanded_position = generate_sin(timer, 1.0, 0.1, static_cast<float>(i) * 0.5);
        data->revolute_servos[i].commanded_velocity = generate_sin_dot(timer, 1.0, 0.1, static_cast<float>(i) * 0.5);
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

std::shared_ptr< msg::sensors_t > Sensors_Message::generate( const int64_t timer )
{
    // sensors
    auto data = std::make_shared<msg::sensors_t>();
    data->timestamp = timer;
    data->imu_sensor_count = this->imu_sensor_count; 
    data->imu_sensors.resize(data->imu_sensor_count);
    
    // lambda: generate sinonoidal signal based on time
    auto generate_sin = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
    {
        float timer_s = static_cast<float>(timer) / 1000.0f;
        return amplitude * std::sin(2.0f * M_PI * frequency * timer_s + phase);
    };

    // lambda: generate time derivative of sinonoidal signal based on time
    auto generate_sin_dot = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
    {
        float timer_s = static_cast<float>( timer ) / 1000.0f;
        return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer_s + phase);
    };

    // lambda: generate white noise signal 
    auto generate_white_noise = [](const float amplitude) -> float
    {
        return amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);
    };  

    for(int i = 0; i < static_cast<int>( data->imu_sensor_count ); i++)
    {
        data->imu_sensors[i].id = i;
        data->imu_sensors[i].quaternion[0] = 1.0;
        data->imu_sensors[i].quaternion[1] = 0.0;
        data->imu_sensors[i].quaternion[2] = 0.0;
        data->imu_sensors[i].quaternion[3] = 0.0;
        data->imu_sensors[i].euler_rate[0] = generate_sin(timer, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.0) + generate_white_noise(0.5);
        data->imu_sensors[i].euler_rate[1] = generate_sin(timer, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.1) + generate_white_noise(0.5);
        data->imu_sensors[i].euler_rate[2] = generate_sin(timer, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.2) + generate_white_noise(0.5);
        data->imu_sensors[i].acceleration[0] = generate_sin(timer, 4.0, 1, static_cast<float>(i) * 0.5 + 0.0) + generate_white_noise(1);
        data->imu_sensors[i].acceleration[1] = generate_sin(timer, 4.0, 1, static_cast<float>(i) * 0.5 + 0.1) + generate_white_noise(1);
        data->imu_sensors[i].acceleration[2] = generate_sin(timer, 4.0, 1, static_cast<float>(i) * 0.5 + 0.2) + generate_white_noise(1);
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

myPublisherLCM::myPublisherLCM()
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

mySubscriberLCM::mySubscriberLCM()
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

void mySubscriberLCM::servosCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::servos_t* msg)
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

void mySubscriberLCM::sensorsCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::sensors_t* msg)
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

myDisplayRT::myDisplayRT( const std::shared_ptr<DisplayRT_Property> monitor_property )
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

DisplayRT::Status myDisplayRT::Setup()
{
    // DisplayRT::defaultSetupDisplay();

    // QObject::connect( this, &myDisplayRT::servosCallbackHandler, this, &myDisplayRT::Update );

    QTimer *dataTimer = new QTimer( this->_app.get() );
    QObject::connect( dataTimer, &QTimer::timeout, this, &myDisplayRT::Update );
    dataTimer->start(50); // Update every 100 ms

    return Status::NORMAL;
}    

DisplayRT::Status myDisplayRT::Update()
{
    this->lcm_->handle();

    // DisplayRT::defaultUpdateDisplay();

    if ( this->servos_data_.get() != nullptr )
    { // servos
        { // integrity check
            // if ( this->servos_data_.get() == nullptr )
            // {
            //     throw std::runtime_error("[WARNING] myDisplayRT: servos data is null");
            // }
            // compare channel name
            if ( this->_monitor_property->WindowProperties()[0]->Channel() != this->channel_servos_ )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT: channel name mismatch, expected " << this->_monitor_property->WindowProperties()[0]->Channel() << " but got " << this->channel_servos_ << std::endl;
                throw std::runtime_error(ss.str());
            }
            // compare servo count
            if ( this->_monitor_property->WindowProperties()[0]->PlotCount() != this->servos_data_->revolute_servo_count )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT: plot count mismatch, expected " << this->_monitor_property->WindowProperties()[0]->PlotCount() << " but got " << this->servos_data_->revolute_servo_count << std::endl;
                throw std::runtime_error(ss.str());
            }
        }

        { // plotting
            auto window = this->_windows[0];
            for ( int i = 0; i < window->Plots().size(); ++i )
            { // plot per servo
                auto plot = window->Plots()[i];
                auto time_s = static_cast<double>( this->servos_data_->timestamp ) / 1000.0;

                { // integrity check
                    if ( plot->Axes().size() != 3 )
                    {
                        std::stringstream ss;
                        ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " has invalid axis count [" << plot->Axes().size() << "]\n";
                        throw std::runtime_error(ss.str());
                    }
                }
                
                { // position
                    auto axis = plot->Axes()[0];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 2 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
                            throw std::runtime_error(ss.str());
                        }
                    }

                    // prepare data
                    // auto time_s = static_cast<double>( this->servos_data_->timestamp ) / 1000.0;
                    auto pos_est = static_cast<double>( this->servos_data_->revolute_servos[i].estimated_position ); 
                    auto pos_cmd = static_cast<double>( this->servos_data_->revolute_servos[i].commanded_position );
                    axis->Axis()->graphs()[0]->addData( time_s, pos_est );
                    axis->Axis()->graphs()[1]->addData( time_s, pos_cmd );
                } 
                
                { // velocity
                    auto axis = plot->Axes()[1];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 2 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
                            throw std::runtime_error(ss.str());
                        }
                    }

                    // prepare data
                    // auto time_s = static_cast<double>( this->servos_data_->timestamp ) / 1000.0;
                    auto vel_est = static_cast<double>( this->servos_data_->revolute_servos[i].estimated_velocity ); 
                    auto vel_cmd = static_cast<double>( this->servos_data_->revolute_servos[i].commanded_velocity );
                    axis->Axis()->graphs()[0]->addData( time_s, vel_est );
                    axis->Axis()->graphs()[1]->addData( time_s, vel_cmd );
                } 

                { // torque
                    auto axis = plot->Axes()[2];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 2 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
                            throw std::runtime_error(ss.str());
                        }
                    }

                    // prepare data
                    // auto time_s = static_cast<double>( this->servos_data_->timestamp ) / 1000.0;
                    auto tor_est = static_cast<double>( this->servos_data_->revolute_servos[i].estimated_torque ); 
                    auto tor_cmd = static_cast<double>( this->servos_data_->revolute_servos[i].commanded_torque );
                    axis->Axis()->graphs()[0]->addData( time_s, tor_est );
                    axis->Axis()->graphs()[1]->addData( time_s, tor_cmd );
                }

                // adjust axis features
                for ( int i = 0; i < plot->Axes().size(); i++ )             
                {
                    auto axis = plot->Axes()[i];
                    const auto xgap = axis->XGap();

                    for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
                    {
                        if ( (time_s-xgap) > 0.0 )
                            axis->Axis()->graphs()[j]->data()->removeBefore(time_s-xgap);
                    }

                    // x-axis range
                    if ( (time_s-xgap) > 0.0 )
                        axis->Axis()->axis(QCPAxis::atBottom)->setRange(time_s-xgap, time_s);
                    
                    // y-axis range
                    if ( axis->IfAutoScale() )
                        axis->Axis()->axis(QCPAxis::atLeft)->rescale(true);
                }
                
            }

            // replot
            for ( auto &plot : window->Plots() ) 
            {
                plot->Plot()->replot();
            }

        }
        
    }

    if ( this->sensors_data_.get() != nullptr )
    { // sensors
        { // integrity check
            // if ( this->sensors_data_.get() == nullptr )
            // {
            //     throw std::runtime_error("[WARNING] myDisplayRT: sensors data is null");
            // }
            // compare channel name
            if ( this->_monitor_property->WindowProperties()[1]->Channel() != this->channel_sensors_ )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT: channel name mismatch, expected " << this->_monitor_property->WindowProperties()[1]->Channel() << " but got " << this->channel_sensors_ << std::endl;
                throw std::runtime_error(ss.str());
            }
            // compare sensor count
            if ( this->_monitor_property->WindowProperties()[1]->PlotCount() != this->sensors_data_->imu_sensor_count )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT: plot count mismatch, expected " << this->_monitor_property->WindowProperties()[1]->PlotCount() << " but got " << this->sensors_data_->imu_sensor_count << std::endl;
                throw std::runtime_error(ss.str());
            }
        }

        { // plotting
            auto window = this->_windows[1];
            for ( int i = 0; i < window->Plots().size(); ++i )
            { // plot per sensor
                auto plot = window->Plots()[i];
                auto time_s = static_cast<double>( this->sensors_data_->timestamp ) / 1000.0;

                { // integrity check
                    if ( plot->Axes().size() != 2 )
                    {
                        std::stringstream ss;
                        ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " has invalid axis count [" << plot->Axes().size() << "]\n";
                        throw std::runtime_error(ss.str());
                    }
                }

                { // linear acceleration
                    auto axis = plot->Axes()[0];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 3 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
                            throw std::runtime_error(ss.str());
                        }
                    }

                    // prepare data
                    auto acc_x = static_cast<double>( this->sensors_data_->imu_sensors[i].acceleration[0] ); 
                    auto acc_y = static_cast<double>( this->sensors_data_->imu_sensors[i].acceleration[1] );
                    auto acc_z = static_cast<double>( this->sensors_data_->imu_sensors[i].acceleration[2] );
                    axis->Axis()->graphs()[0]->addData( time_s, acc_x );
                    axis->Axis()->graphs()[1]->addData( time_s, acc_y );
                    axis->Axis()->graphs()[2]->addData( time_s, acc_z );
                } 
                
                { // angular velocity
                    auto axis = plot->Axes()[1];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 3 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
                            throw std::runtime_error(ss.str());
                        }
                    }

                    // prepare data
                    auto vel_x = static_cast<double>( this->sensors_data_->imu_sensors[i].euler_rate[0] ); 
                    auto vel_y = static_cast<double>( this->sensors_data_->imu_sensors[i].euler_rate[1] );
                    auto vel_z = static_cast<double>( this->sensors_data_->imu_sensors[i].euler_rate[2] );
                    axis->Axis()->graphs()[0]->addData( time_s, vel_x );
                    axis->Axis()->graphs()[1]->addData( time_s, vel_y );
                    axis->Axis()->graphs()[2]->addData( time_s, vel_z );
                } 

                // adjust axis features
                for ( int i = 0; i < plot->Axes().size(); i++ )             
                {
                    auto axis = plot->Axes()[i];
                    const auto xgap = axis->XGap();

                    for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
                    {
                        if ( (time_s-xgap) > 0.0 )
                            axis->Axis()->graphs()[j]->data()->removeBefore(time_s-xgap);
                    }

                    // x-axis range
                    if ( (time_s-xgap) > 0.0 )
                        axis->Axis()->axis(QCPAxis::atBottom)->setRange(time_s-xgap, time_s);
                    
                    // y-axis range
                    if ( axis->IfAutoScale() )
                        axis->Axis()->axis(QCPAxis::atLeft)->rescale(true);
                }
            }  

            // replot
            for ( auto &plot : window->Plots() ) 
            {
                plot->Plot()->replot();
            }
        }

    }

    return Status::NORMAL;
}

void myDisplayRT::servosCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::servos_t* msg)
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

void myDisplayRT::sensorsCallbackHandler(const lcm::ReceiveBuffer* rbuf, const std::string& channel, const msg::sensors_t* msg)
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

}// namespace display_rt::example

