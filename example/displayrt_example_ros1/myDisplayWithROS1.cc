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

// #include <lcm/lcm-cpp.hpp>
// #include "msg/servos_t.hpp"
// #include "msg/sensors_t.hpp"

#include "myDisplayWithROS1.h"


using namespace display_rt; // for DisplayRT

namespace display_rt::example {

/***************/
/* LCM Message */
/***************/

// std::shared_ptr< msg::servos_t > Servos_Message::generate( const int64_t timer )
// {
//     // servos
//     auto data = std::make_shared<msg::servos_t>();
//     data->timestamp = timer;
//     data->revolute_servo_count = this->revolute_servo_count; 
//     data->revolute_servos.resize(data->revolute_servo_count);
    
//     // lambda: generate sinonoidal signal based on time
//     auto generate_sin = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
//     {
//         float timer_s = static_cast<float>(timer) / 1000.0f;
//         return amplitude * std::sin(2.0f * M_PI * frequency * timer + phase);
//     };

//     // lambda: generate time derivative of sinonoidal signal based on time
//     auto generate_sin_dot = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
//     {
//         float timer_s = timer / 1000.0f;
//         return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer + phase);
//     };

//     // lambda: generate white noise signal 
//     auto generate_white_noise = [](const float amplitude) -> float
//     {
//         return amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);
//     };

//     for(int i = 0; i < static_cast<int>( data->revolute_servo_count ); i++)
//     {
//         data->revolute_servos[i].id = i;

//         data->revolute_servos[i].commanded_position = generate_sin(timer, 1.0, 0.1, static_cast<float>(i) * 0.5);
//         data->revolute_servos[i].commanded_velocity = generate_sin_dot(timer, 1.0, 0.1, static_cast<float>(i) * 0.5);
//         data->revolute_servos[i].commanded_torque = 0.5 * data->revolute_servos[i].commanded_position; 

//         data->revolute_servos[i].estimated_position = data->revolute_servos[i].commanded_position + generate_white_noise(0.1);
//         data->revolute_servos[i].estimated_velocity = data->revolute_servos[i].commanded_velocity + generate_white_noise(0.1);
//         data->revolute_servos[i].estimated_torque = data->revolute_servos[i].commanded_torque + generate_white_noise(0.1);
//     }

//     { // debug
//         std::stringstream ss;
//         ss << "Publishing Servos Message : \n" 
//         << "channel: " << channel << ","
//         << "timestamp: " << data->timestamp << std::endl;
        
//         for(int i = 0; i < static_cast<int>( data->revolute_servo_count); ++i)
//         {
//         ss << "revolute servo[" << i << "]: "
//             << "id: " << static_cast<int>( data->revolute_servos[i].id ) << ","
//             << "estimated position: " << data->revolute_servos[i].estimated_position << ","
//             << "velocity: " << data->revolute_servos[i].estimated_velocity << ","
//             << "torque: " << data->revolute_servos[i].estimated_torque << "; "
//             << "commanded position: " << data->revolute_servos[i].commanded_position << ","
//             << "velocity: " << data->revolute_servos[i].commanded_velocity << ","
//             << "torque: " << data->revolute_servos[i].commanded_torque 
//             << std::endl;
//         }
        
//         std::cout << ss.str() << std::endl;
//     }

//     return data;
// }

// std::shared_ptr< msg::sensors_t > Sensors_Message::generate( const int64_t timer )
// {
//     // sensors
//     auto data = std::make_shared<msg::sensors_t>();
//     data->timestamp = timer;
//     data->imu_sensor_count = this->imu_sensor_count; 
//     data->imu_sensors.resize(data->imu_sensor_count);
    
//     // lambda: generate sinonoidal signal based on time
//     auto generate_sin = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
//     {
//         float timer_s = static_cast<float>(timer) / 1000.0f;
//         return amplitude * std::sin(2.0f * M_PI * frequency * timer_s + phase);
//     };

//     // lambda: generate time derivative of sinonoidal signal based on time
//     auto generate_sin_dot = [](const int64_t timer, const float amplitude, const float frequency, const float phase) -> float
//     {
//         float timer_s = static_cast<float>( timer ) / 1000.0f;
//         return amplitude * 2.0f * M_PI * frequency * std::cos(2.0f * M_PI * frequency * timer_s + phase);
//     };

//     // lambda: generate white noise signal 
//     auto generate_white_noise = [](const float amplitude) -> float
//     {
//         return amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);
//     };  

//     for(int i = 0; i < static_cast<int>( data->imu_sensor_count ); i++)
//     {
//         data->imu_sensors[i].id = i;
//         data->imu_sensors[i].quaternion[0] = 1.0;
//         data->imu_sensors[i].quaternion[1] = 0.0;
//         data->imu_sensors[i].quaternion[2] = 0.0;
//         data->imu_sensors[i].quaternion[3] = 0.0;
//         data->imu_sensors[i].euler_rate[0] = generate_sin(timer, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.0) + generate_white_noise(0.5);
//         data->imu_sensors[i].euler_rate[1] = generate_sin(timer, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.1) + generate_white_noise(0.5);
//         data->imu_sensors[i].euler_rate[2] = generate_sin(timer, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.2) + generate_white_noise(0.5);
//         data->imu_sensors[i].acceleration[0] = generate_sin(timer, 4.0, 1, static_cast<float>(i) * 0.5 + 0.0) + generate_white_noise(1);
//         data->imu_sensors[i].acceleration[1] = generate_sin(timer, 4.0, 1, static_cast<float>(i) * 0.5 + 0.1) + generate_white_noise(1);
//         data->imu_sensors[i].acceleration[2] = generate_sin(timer, 4.0, 1, static_cast<float>(i) * 0.5 + 0.2) + generate_white_noise(1);
//     }

//     { // debug
//         std::stringstream ss;
//         ss << "Publishing Sensors Message : \n" 
//                 << "channel: " << channel << ","
//                 << "timestamp: " << data->timestamp << std::endl;
        
//         for(int i = 0; i < static_cast<int>( data->imu_sensor_count ); ++i)
//         {
//             ss << "imu sensor[" << i << "]: "
//                 << "id: " << static_cast<int>( data->imu_sensors[i].id ) << ","
//                 << "quaternion: " << data->imu_sensors[i].quaternion[0] << ", " << data->imu_sensors[i].quaternion[1] << ", " << data->imu_sensors[i].quaternion[2] << ", " << data->imu_sensors[i].quaternion[3] << ";"
//                 << "euler rate: " << data->imu_sensors[i].euler_rate[0] << ", " << data->imu_sensors[i].euler_rate[1] << ", " << data->imu_sensors[i].euler_rate[2] << ";"
//                 << "acceleration: " << data->imu_sensors[i].acceleration[0] << ", " << data->imu_sensors[i].acceleration[1] << ", " << data->imu_sensors[i].acceleration[2] 
//                 << std::endl;
//         }
        
//         std::cout << ss.str() << std::endl;
//     }

//     return data;
// }

myPublisherROS1::myPublisherROS1()
{
    this->node_handle_ = std::make_shared<ros::NodeHandle>();
    this->publisher_sensors_ = std::make_shared<ros::Publisher>(this->node_handle_->advertise<displayrt_example_ros1::Sensors>(this->channel_sensors_, 10));
    this->publisher_servos_ = std::make_shared<ros::Publisher>(this->node_handle_->advertise<displayrt_example_ros1::Servos>(this->channel_servos_, 10));

    // Capture the start time
    start_time_ = std::chrono::steady_clock::now();
}

void myPublisherROS1::publishServos()
{
    const auto timestamp = this->getElapsedTimeInMilliseconds();

    auto message = displayrt_example_ros1::Servos();
    message.timestamp = timestamp;
    message.revolute_servo_count = this->count_revolute_servo_;

    // loop through the revolute servos
    for (size_t i = 0; i < message.revolute_servo_count; i++)
    {
        auto servo_message = displayrt_example_ros1::RevoluteServo();
        servo_message.timestamp = timestamp; 
        servo_message.id = i;
        servo_message.parent_id = 0;
        servo_message.enabled = true;

        servo_message.commanded_position = generateSin(timestamp, 1.0, 0.5, static_cast<float>(i) * M_PI_4);
        servo_message.commanded_velocity = generateSinDot(timestamp, 1.0, 0.5, static_cast<float>(i) * M_PI_4);
        servo_message.commanded_torque = 0.5 * servo_message.commanded_position; 

        servo_message.estimated_position = servo_message.commanded_position + generateWhiteNoise(0.5);
        servo_message.estimated_velocity = servo_message.commanded_velocity + generateWhiteNoise(1.0);
        servo_message.estimated_torque = servo_message.commanded_torque + generateWhiteNoise(0.1);

        message.revolute_servos.push_back(servo_message);
    }

    ROS_INFO("Publishing: '%d'", message.timestamp);
    this->publisher_servos_->publish(message);
}

void myPublisherROS1::publishSensors()
{
    const auto timestamp = this->getElapsedTimeInMilliseconds();

    auto message = displayrt_example_ros1::Sensors();
    message.timestamp = timestamp;
    message.imu_sensor_count = this->count_imu_sensor_;

    // loop through the imu sensors
    for (size_t i = 0; i < message.imu_sensor_count; i++)
    {
        auto imu_message = displayrt_example_ros1::IMU();
        imu_message.timestamp = timestamp;
        imu_message.id = i;
        imu_message.parent_id = 0;
        imu_message.quaternion[0] = 1.0;
        imu_message.quaternion[1] = 0.0;
        imu_message.quaternion[2] = 0.0;
        imu_message.quaternion[3] = 0.0;
        imu_message.euler_rate[0] = generateSin(timestamp, 2.0, 0.5, static_cast<float>(i) * 0.5 + 0.0*M_PI_4) + generateWhiteNoise(0.5);
        imu_message.euler_rate[1] = generateSin(timestamp, 2.0, 0.5, static_cast<float>(i) * 0.5 + 1.0*M_PI_4) + generateWhiteNoise(0.5);
        imu_message.euler_rate[2] = generateSin(timestamp, 2.0, 0.5, static_cast<float>(i) * 0.5 + 2.0*M_PI_4) + generateWhiteNoise(0.5);
        imu_message.acceleration[0] = generateSin(timestamp, 4.0, 1, static_cast<float>(i) * 0.5 + 0.0*M_PI_4) + generateWhiteNoise(1);
        imu_message.acceleration[1] = generateSin(timestamp, 4.0, 1, static_cast<float>(i) * 0.5 + 1.0*M_PI_4) + generateWhiteNoise(1);
        imu_message.acceleration[2] = generateSin(timestamp, 4.0, 1, static_cast<float>(i) * 0.5 + 2.0*M_PI_4) + generateWhiteNoise(1);

        message.imu_sensors.push_back(imu_message);
    }

    ROS_INFO("Publishing: '%d'", message.timestamp);
    this->publisher_sensors_->publish(message);
}

mySubscriberROS1::mySubscriberROS1()
{
    this->node_handle_ = std::make_shared<ros::NodeHandle>();
    this->subscriber_servos_ = std::make_shared<ros::Subscriber>(this->node_handle_->subscribe(this->channel_servos_, 10, &mySubscriberROS1::servosCallbackHandler, this));
    this->subscriber_sensors_ = std::make_shared<ros::Subscriber>(this->node_handle_->subscribe(this->channel_sensors_, 10, &mySubscriberROS1::sensorsCallbackHandler, this));

    this->servos_data_ = std::make_shared<displayrt_example_ros1::Servos>();
    this->sensors_data_ = std::make_shared<displayrt_example_ros1::Sensors>();
}

void mySubscriberROS1::servosCallbackHandler(const displayrt_example_ros1::Servos::ConstPtr &msg)
{
    this->servos_data_ = std::make_shared<displayrt_example_ros1::Servos>(*msg);
    
    { // debug
        std::stringstream ss;
        ss << "Receiving Servos Message : \n" 
            << "channel: " << this->channel_servos_ << ","
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

void mySubscriberROS1::sensorsCallbackHandler(const displayrt_example_ros1::Sensors::ConstPtr &msg)
{
    this->sensors_data_ = std::make_shared<displayrt_example_ros1::Sensors>(*msg);
    
    { // debug
        std::stringstream ss;
        ss << "Receiving Sensors Message : \n" 
            << "channel: " << this->channel_sensors_ << ","
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

myDisplayRT_ROS2::myDisplayRT_ROS2( const std::shared_ptr<DisplayRT_Property> monitor_property )
: DisplayRT(monitor_property)
{
    this->node_handle_ = std::make_shared<ros::NodeHandle>();
    this->subscriber_servos_ = std::make_shared<ros::Subscriber>(this->node_handle_->subscribe(this->channel_servos_, 10, &myDisplayRT_ROS2::servosCallbackHandler, this));
    this->subscriber_sensors_ = std::make_shared<ros::Subscriber>(this->node_handle_->subscribe(this->channel_sensors_, 10, &myDisplayRT_ROS2::sensorsCallbackHandler, this));

    this->servos_data_ = std::make_shared<displayrt_example_ros1::Servos>();
    this->sensors_data_ = std::make_shared<displayrt_example_ros1::Sensors>();
}

DisplayRT::Status myDisplayRT_ROS2::Setup()
{
    // DisplayRT::defaultSetupDisplay();

    // QObject::connect( this, &myDisplayRT_ROS2::servosCallbackHandler, this, &myDisplayRT_ROS2::Update );

    QTimer *dataTimer = new QTimer( this->_app.get() );
    QObject::connect( dataTimer, &QTimer::timeout, this, &myDisplayRT_ROS2::Update );
    dataTimer->start(10); // Update every 10 ms

    return Status::NORMAL;
}    

DisplayRT::Status myDisplayRT_ROS2::Update()
{
    if( ros::ok() )
    {
        ros::spinOnce();
    }

    // rclcpp::spin_some(this->shared_from_this());

    // DisplayRT::defaultUpdateDisplay();

    if ( this->servos_data_.get() != nullptr )
    { // servos
        { // integrity check
            // if ( this->servos_data_.get() == nullptr )
            // {
            //     throw std::runtime_error("[WARNING] myDisplayRT_ROS2: servos data is null");
            // }
            // compare channel name
            if ( this->_monitor_property->WindowProperties()[0]->Channel() != this->channel_servos_ )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT_ROS2: channel name mismatch, expected " << this->_monitor_property->WindowProperties()[0]->Channel() << " but got " << this->channel_servos_ << std::endl;
                throw std::runtime_error(ss.str());
            }
            // compare servo count
            if ( this->_monitor_property->WindowProperties()[0]->PlotCount() != this->servos_data_->revolute_servo_count )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT_ROS2: plot count mismatch, expected " << this->_monitor_property->WindowProperties()[0]->PlotCount() << " but got " << this->servos_data_->revolute_servo_count << std::endl;
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
                        ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " has invalid axis count [" << plot->Axes().size() << "]\n";
                        throw std::runtime_error(ss.str());
                    }
                }
                
                { // position
                    auto axis = plot->Axes()[0];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 2 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
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
                            ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
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
                            ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
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
            //     throw std::runtime_error("[WARNING] myDisplayRT_ROS2: sensors data is null");
            // }
            // compare channel name
            if ( this->_monitor_property->WindowProperties()[1]->Channel() != this->channel_sensors_ )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT_ROS2: channel name mismatch, expected " << this->_monitor_property->WindowProperties()[1]->Channel() << " but got " << this->channel_sensors_ << std::endl;
                throw std::runtime_error(ss.str());
            }
            // compare sensor count
            if ( this->_monitor_property->WindowProperties()[1]->PlotCount() != this->sensors_data_->imu_sensor_count )
            {
                std::stringstream ss;
                ss << "[WARNING] myDisplayRT_ROS2: plot count mismatch, expected " << this->_monitor_property->WindowProperties()[1]->PlotCount() << " but got " << this->sensors_data_->imu_sensor_count << std::endl;
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
                        ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " has invalid axis count [" << plot->Axes().size() << "]\n";
                        throw std::runtime_error(ss.str());
                    }
                }

                { // linear acceleration
                    auto axis = plot->Axes()[0];

                    { // integrity check
                        if ( axis->Axis()->graphs().size() != 3 )
                        {
                            std::stringstream ss;
                            ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
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
                            ss << "[WARNING] myDisplayRT_ROS2: plot " << plot->Id() << " axis " << axis->Id() << " has invalid graph count [" << axis->Axis()->graphs().size() << "]\n";
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

void myDisplayRT_ROS2::servosCallbackHandler(const displayrt_example_ros1::Servos::ConstPtr &msg)
{
    this->servos_data_ = std::make_shared<displayrt_example_ros1::Servos>(*msg);
    
    { // debug
        std::stringstream ss;
        ss << "Receiving Servos Message : \n" 
            << "channel: " << this->channel_servos_ << ","
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

void myDisplayRT_ROS2::sensorsCallbackHandler(const displayrt_example_ros1::Sensors::ConstPtr &msg)
{
    this->sensors_data_ = std::make_shared<displayrt_example_ros1::Sensors>(*msg);
    
    { // debug
        std::stringstream ss;
        ss << "Receiving Sensors Message : \n" 
            << "channel: " << this->channel_sensors_ << ","
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

} // namespace display_rt::example

