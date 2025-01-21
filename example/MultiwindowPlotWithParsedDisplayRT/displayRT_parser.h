// Authors:
// Keran Ye <kye007@ucr.edu>

#pragma once

#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <chrono>

// // Eigen
// #include "Eigen/Dense"
// #include "Eigen/Geometry"

#include "displayRT.h"

namespace display_rt {

class DisplayRT_Parser: public DisplayRT_ParserBase {
  public: // types

  public: // major methods and implementations of virtual methods

    DisplayRT_Parser()
    : DisplayRT_ParserBase() 
    {}

    std::shared_ptr<DisplayRT_Property> parseConfiguration(
      const std::string &config_file_path, Yaml::Node &parser_in
    ) override 
    {
      using DisplayProperty = DisplayRT_Property; 
      using WindowProperty = QCP_Window_Property;
      using PlotProperty = QCP_Plot_Property;
      using AxisProperty = QCP_Axis_Property;
      using GraphProperty = QCP_Graph_Property;
      using PenProperty = QCP_Pen_Property;

      auto & parser = getParser( config_file_path, parser_in );

      auto & messages_yaml = parser["monitor"]["Monitor0"]["messages"];

      // // check if the configuration is QCP compatible
      // auto config_ptr = std::dynamic_pointer_cast<DisplayProperty>(configuration);
      // if ( config_ptr.get() == nullptr ) {
      //   std::stringstream ss;
      //   ss << "\n\nMonitor_Parser_Abstraction::parseMonitorConfiguration: incompatible monitor configuration. ";
      //   ss << "Expected QCP_Monitor_Property, but got something else.\n\n";
      //   throw std::runtime_error(ss.str());
      //   return ControlStatus::ErrorTerminate;
      // }

      // lambda: parse window property
      auto parseWindowProperty = [] ( Yaml::Node & window_yaml ) {
        bool enable = window_yaml["enable"].As<bool>();
        if ( enable ) {
          // window property
          int window_id = window_yaml["id"].As<int>();
          std::string channel = window_yaml["channel"].As<std::string>();
          int px = window_yaml["px"].As<int>(); // window position x
          int py = window_yaml["py"].As<int>(); // window position y
          int width = window_yaml["width"].As<int>(); // window width
          int height = window_yaml["height"].As<int>(); // window height
          int plot_count = window_yaml["plot_count"].As<int>(); // number of plots under the same window
          
          // plot properties
          std::vector< std::shared_ptr<PlotProperty> > plot_properties;

          // loop over plots
          auto & plots_yaml = window_yaml["plots"];
          for ( int i = 0; i < plot_count; i++ ) {
            std::shared_ptr< PlotProperty > plot_property;
            
            auto & plot_yaml = plots_yaml[i];

            int plot_id = plot_yaml["id"].As<int>();
            int axis_count = plot_yaml["axis_count"].As<int>();
            std::string title = plot_yaml["title"].As<std::string>();

            // axis properties
            std::vector< std::shared_ptr<AxisProperty> > axis_properties;

            // loop over axes
            auto & axes_yaml = plot_yaml["axes"];
            for ( int j = 0; j < axis_count; j++ ) {
              std::shared_ptr< AxisProperty > axis_property;

              auto & axis_yaml = axes_yaml[j];

              int axis_id = axis_yaml["id"].As<int>();
              int graph_count = axis_yaml["graph_count"].As<int>();
              std::string title = axis_yaml["title"].As<std::string>();
              std::string x_label = axis_yaml["x_label"].As<std::string>();
              std::string y_label = axis_yaml["y_label"].As<std::string>();
              double x_gap = axis_yaml["x_gap"].As<double>();
              double y_min = axis_yaml["y_min"].As<double>();
              double y_max = axis_yaml["y_max"].As<double>();
              bool if_auto_scale = axis_yaml["if_auto_scale"].As<bool>();

              // graph properties
              std::vector< std::shared_ptr<GraphProperty> > graph_properties;

              // loop over graphs
              auto & graphs_yaml = axis_yaml["graphs"];
              for ( int k = 0; k < graph_count; k++ ) {
                std::shared_ptr< GraphProperty > graph_property;

                auto & graph_yaml = graphs_yaml[k];

                int graph_id = graph_yaml["id"].As<int>();
                std::string name = graph_yaml["name"].As<std::string>();
                std::string line_style = graph_yaml["line_style"].As<std::string>();

                // pen property
                std::shared_ptr< PenProperty > pen_property;
                { // pen
                  std::string color = graph_yaml["pen_color"].As<std::string>();
                  std::string style = graph_yaml["pen_style"].As<std::string>();
                  int width = graph_yaml["pen_width"].As<int>();
                  pen_property = std::make_shared< PenProperty >(color, style, width);
                }

                graph_property = std::make_shared< GraphProperty >(graph_id, name, line_style, pen_property);

                // store graph property
                graph_properties.push_back( graph_property );
              }

              axis_property = std::make_shared< AxisProperty >(axis_id, graph_count, title, x_label, y_label, x_gap, y_min, y_max, if_auto_scale, graph_properties);

              // store axis property
              axis_properties.push_back( axis_property );
            }

            plot_property = std::make_shared< PlotProperty >(plot_id, axis_count, title, axis_properties);

            // store plot property
            plot_properties.push_back( plot_property );
          }
          
          auto window_property = std::make_shared< WindowProperty >(window_id, channel, plot_count, px, py, width, height, plot_properties);

          // // store window property
          // window_properties.push_back( window_property );

          return window_property; 
        } 

        return std::shared_ptr<QCP_Window_Property>(nullptr);
      }; // parseWindowProperty

      // monitor property
      auto & monitor_yaml = parser["monitor"]["Monitor0"]; 
      int monitor_id = monitor_yaml["id"].As<int>();
      int window_count = monitor_yaml["window_count"].As<int>();
      
      // window properties
      std::vector< std::shared_ptr<WindowProperty> > window_properties;

      { // robot: servo message
        auto & window_yaml = messages_yaml["robot"]["servos"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // robot: sensor message
        auto & window_yaml = messages_yaml["robot"]["sensors"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // robot: bodies
        auto & window_yaml = messages_yaml["robot"]["bodies"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // robot: legs
        auto & window_yaml = messages_yaml["robot"]["legs"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      // more robot-related components

      { // controller: supervisor
        auto & window_yaml = messages_yaml["controller"]["supervisor"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: composite_centroid
        auto & window_yaml = messages_yaml["controller"]["composite_centroid"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: gait_scheduler
        auto & window_yaml = messages_yaml["controller"]["gait_scheduler"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: low-level estimator
        auto & window_yaml = messages_yaml["controller"]["low_level_estimator"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: whole-body estimator
        auto & window_yaml = messages_yaml["controller"]["whole_body_estimator"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: whole-body planner
        auto & window_yaml = messages_yaml["controller"]["whole_body_planner"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: stance-leg planner
        auto & window_yaml = messages_yaml["controller"]["stance_leg_planner"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: swing-leg planner
        auto & window_yaml = messages_yaml["controller"]["swing_leg_planner"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      { // controller: low-level tracker
        auto & window_yaml = messages_yaml["controller"]["low_level_tracker"];
        auto window_property = parseWindowProperty( window_yaml );
        if ( window_property != nullptr ) {
          window_properties.push_back( window_property );
        }
      }

      

      // more controller-related components

      


      // store monitor property
      auto monitor_property = std::make_shared< DisplayProperty >(monitor_id, window_count, window_properties);
      // auto monitor_property_ptr = std::dynamic_pointer_cast<Monitor_Abstraction::MonitorConfig>(monitor_property);
      // if ( monitor_property_ptr.get() == nullptr ) {
      //   std::stringstream ss;
      //   ss << "\n\nMonitor_Parser_Abstraction::parseMonitorConfiguration: incompatible monitor configuration. ";
      //   ss << "Expected QCP_Monitor_Property inherits from Monitor_Abstraction::MonitorConfig, but got something else.\n\n";
      //   throw std::runtime_error(ss.str());
      //   return nullptr;
      // }

      return monitor_property;
    } 

}; // SCIQ2_Monitor_Parser_QCP


} // namespace display_rt