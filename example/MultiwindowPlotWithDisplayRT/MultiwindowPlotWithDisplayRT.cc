/*
 * This file is part of DisplayRT.
 *
 * Copyright (C) 2024-2025 Keran Ye <keran.ye@email.ucr.edu>
 *
 * DisplayRT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DisplayRT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DisplayRT. If not, see <https://www.gnu.org/licenses/>.
 *
 * Author(s): 
 * - Keran Ye (keran.ye@email.ucr.edu)
 * 
 * The content of this file is about testing multiple-window display using DisplayRT. 
 * The display property is manually defined in the main function. 
 */

#include <memory>
#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>

// #ifndef QCUSTOMPLOT_USE_LIBRARY
// #define QCUSTOMPLOT_USE_LIBRARY
// #endif

#include "qcustomplot.h"

#include "displayRT.h"

using namespace display_rt; // for DisplayRT

class myDisplayRT : public DisplayRT
{
public: 
    myDisplayRT( const std::shared_ptr<DisplayRT_Property> display_property )
    : DisplayRT(display_property)
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
};

int main(int argc, char *argv[])
{
    std::shared_ptr<DisplayRT_Property> display_property; 
    int id = 0;
    int window_count = 2;
    std::vector< std::shared_ptr<QCP_Window_Property> > window_properties;
    
    // prepare monitor properties
    // std::shared_ptr<QCP_Window_Property> window_property; 
    { // window 0
        std::shared_ptr<QCP_Window_Property> window_property; 

        int id = 0;
        std::string channel = "channel 0";
        int plot_count = 2;
        int width = 800;
        int height = 600;
        std::vector< std::shared_ptr< QCP_Plot_Property > > plot_properties;
        
        // std::shared_ptr< QCP_Plot_Property > plot_property;
        { // plot 0
            std::shared_ptr< QCP_Plot_Property > plot_property;

            int id = 0;
            int axis_count = 2;
            std::string title = "plot 0";
            std::vector< std::shared_ptr<QCP_Axis_Property> > axis_properties;

            // std::shared_ptr< QCP_Axis_Property > axis_property;
            { // axis 0
                std::shared_ptr< QCP_Axis_Property > axis_property;

                int id = 0;
                int graph_count = 2;
                std::string title = "axis 0";
                std::string x_label = "axis 0 x";
                std::string y_label = "axis 0 y";
                double x_gap = 5; 
                double y_min = -2;
                double y_max = 2;
                bool if_auto_scale = true;
                std::vector< std::shared_ptr<QCP_Graph_Property> > graph_properties; 
                
                // std::shared_ptr< QCP_Graph_Property > graph_property;
                { // graph 0
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 0;
                    std::string name = "graph 0";
                    std::string line_style = "line";
                    
                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "black";
                        std::string style = "solid";
                        int width = 1;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                { // graph 1
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 1;
                    std::string name = "graph 1";
                    std::string line_style = "line";
                    
                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "red";
                        std::string style = "dash";
                        int width = 2;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                axis_property = std::make_shared< QCP_Axis_Property >(id, graph_count, title, x_label, y_label, x_gap, y_min, y_max, if_auto_scale, graph_properties);

                axis_properties.push_back(axis_property);
            }
            // axis_properties.push_back(axis_property);

            { // axis 1
                std::shared_ptr< QCP_Axis_Property > axis_property;

                int id = 1; 
                int graph_count = 1;
                std::string title = "axis 1";
                std::string x_label = "axis 1 x";
                std::string y_label = "axis 1 y";
                double x_gap = 5; 
                double y_min = -5;
                double y_max = 5;
                bool if_auto_scale = false;
                std::vector< std::shared_ptr<QCP_Graph_Property> > graph_properties; 
                
                // std::shared_ptr< QCP_Graph_Property > graph_property; 
                { // graph 0    
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 0;
                    std::string name = "graph 0";
                    std::string line_style = "line";
                    
                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "blue";
                        std::string style = "solid";
                        int width = 4;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                axis_property = std::make_shared< QCP_Axis_Property >(id, graph_count, title, x_label, y_label, x_gap, y_min, y_max, if_auto_scale, graph_properties);
            
                axis_properties.push_back(axis_property);
            }
            // axis_properties.push_back(axis_property);

            plot_property = std::make_shared< QCP_Plot_Property >(id, axis_count, title, axis_properties);
        
            plot_properties.push_back(plot_property);
        }
        // plot_properties.push_back(plot_property); 

        { // plot 1
            std::shared_ptr< QCP_Plot_Property > plot_property;

            int id = 1;
            int axis_count = 1;
            std::string title = "plot 1";
            std::vector< std::shared_ptr<QCP_Axis_Property> > axis_properties;
                
            // std::shared_ptr< QCP_Axis_Property > axis_property; 
            { // axis 0
                std::shared_ptr< QCP_Axis_Property > axis_property;

                int id = 0; 
                int graph_count = 3;
                std::string title = "axis 0";
                std::string x_label = "axis 0 x";
                std::string y_label = "axis 0 y";
                double x_gap = 10; 
                double y_min = -2;
                double y_max = 2;
                bool if_auto_scale = false;
                std::vector< std::shared_ptr<QCP_Graph_Property> > graph_properties; 

                // std::shared_ptr< QCP_Graph_Property > graph_property; 
                { // graph 0
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 0;
                    std::string name = "graph 0";
                    std::string line_style = "line";
                    
                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "blue";
                        std::string style = "solid";
                        int width = 1;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                { // graph 1
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 1;
                    std::string name = "graph 1";
                    std::string line_style = "line";
                    
                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "red";
                        std::string style = "dash";
                        int width = 2;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                { // graph 2
                    std::shared_ptr< QCP_Graph_Property > graph_property; 

                    int id = 2;
                    std::string name = "graph 2";
                    std::string line_style = "line";
                    
                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "black";
                        std::string style = "dash";
                        int width = 2;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                axis_property = std::make_shared< QCP_Axis_Property >(id, graph_count, title, x_label, y_label, x_gap, y_min, y_max, if_auto_scale, graph_properties);

                axis_properties.push_back(axis_property);
            }
            // axis_properties.push_back(axis_property);

            plot_property = std::make_shared< QCP_Plot_Property >(id, axis_count, title, axis_properties);
        
            plot_properties.push_back(plot_property);
        }
        // plot_properties.push_back(plot_property);

        window_property = std::make_shared<QCP_Window_Property>(id, channel, plot_count, width, height, plot_properties);

        window_properties.push_back(window_property);
    }
    // window_properties.push_back(window_property);

    { // window 1
        std::shared_ptr<QCP_Window_Property> window_property; 

        int id = 1;
        std::string channel = "channel 1";
        int plot_count = 1;
        int width = 800;
        int height = 600;
        std::vector< std::shared_ptr< QCP_Plot_Property > > plot_properties;
        
        // std::shared_ptr< QCP_Plot_Property > plot_property;
        { // plot 0
            std::shared_ptr< QCP_Plot_Property > plot_property;

            int id = 0;
            int axis_count = 1;
            std::string title = "plot 0";
            std::vector< std::shared_ptr<QCP_Axis_Property> > axis_properties;

            // std::shared_ptr< QCP_Axis_Property > axis_property;
            { // axis 0
                std::shared_ptr< QCP_Axis_Property > axis_property;

                int id = 0;
                int graph_count = 3;
                std::string title = "axis 0";
                std::string x_label = "axis 0 x";
                std::string y_label = "axis 0 y";
                double x_gap = 5;
                double y_min = -2;
                double y_max = 2;
                bool if_auto_scale = true;
                std::vector< std::shared_ptr<QCP_Graph_Property> > graph_properties;

                // std::shared_ptr< QCP_Graph_Property > graph_property;
                { // graph 0
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 0;
                    std::string name = "graph 0";
                    std::string line_style = "line";

                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "black";
                        std::string style = "solid";
                        int width = 1;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);
                    
                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                { // graph 1
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 1;
                    std::string name = "graph 1";
                    std::string line_style = "line";

                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "red";
                        std::string style = "dash";
                        int width = 2;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                { // graph 2
                    std::shared_ptr< QCP_Graph_Property > graph_property;

                    int id = 2;
                    std::string name = "graph 2";
                    std::string line_style = "line";

                    std::shared_ptr< QCP_Pen_Property > pen_property;
                    { // pen
                        std::string color = "blue";
                        std::string style = "dash";
                        int width = 2;
                        pen_property = std::make_shared< QCP_Pen_Property >(color, style, width);
                    }

                    graph_property = std::make_shared< QCP_Graph_Property >(id, name, line_style, pen_property);

                    graph_properties.push_back(graph_property);
                }
                // graph_properties.push_back(graph_property);

                axis_property = std::make_shared< QCP_Axis_Property >(id, graph_count, title, x_label, y_label, x_gap, y_min, y_max, if_auto_scale, graph_properties);

                axis_properties.push_back(axis_property);
            }
            // axis_properties.push_back(axis_property);

            plot_property = std::make_shared< QCP_Plot_Property >(id, axis_count, title, axis_properties);
        
            plot_properties.push_back(plot_property);
        }
        // plot_properties.push_back(plot_property);

        window_property = std::make_shared<QCP_Window_Property>(id, channel, plot_count, width, height, plot_properties);

        window_properties.push_back(window_property);
    }
    // window_properties.push_back(window_property);

    display_property = std::make_shared<DisplayRT_Property>(id, window_count, window_properties);
    
    // display
    auto display = std::make_shared<myDisplayRT>( display_property ); 
    display->Initial( argc, argv); 
    display->Setup();
    display->Start(); 

    return 0; 
}