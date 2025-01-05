/*
 * This file is part of RT_Display.
 *
 * Copyright (C) 2024-2025 Keran Ye <keran.ye@email.ucr.edu>
 *
 * RT_Display is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RT_Display is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RT_Display. If not, see <https://www.gnu.org/licenses/>.
 *
 * Author(s): 
 * - Keran Ye (keran.ye@email.ucr.edu)
 */


#pragma once

// Standard Includes
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <iostream>

// QT5
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>

// QCustomPlot
#include "qcustomplot.h"

// #include "parser/abstraction_parser.h"

namespace rt_display {

/***************************/
/******** Properties *******/
/***************************/

class QCP_Pen_Property {
public: // major methods
    
    /**
     * @brief Constructor for QCP Pen Property
     * 
     * @param color pen color, that is, line color
     * @param style pen style, that is, line style
     * @param width pen width, that is, line width
     */
    QCP_Pen_Property( 
        const std::string &color,
        const std::string &style,
        const int width
    ); 

    // QCP_Pen_Property() {}; 

private: // internal variables
    Qt::GlobalColor _color; // pen color, that is, line color
    Qt::PenStyle _style; // pen style, that is, line style
    int _width; // pen width, that is, line width

private: // internal methods
    void setColor( const std::string &color );
    void setStyle( const std::string &style );
    void setWidth( const int width );

public: // accessors
    Qt::GlobalColor Color() const { return _color; }
    Qt::PenStyle Style() const { return _style; }
    int Width() const { return _width; }

}; // class QCP_Pen_Property

class QCP_Graph_Property {
public: // major methods
    
    /**
     * @brief Constructor for QCP Graph Property
     * 
     * @param id id for graph, used for local index under the same axis
     * @param pen_property pen property for graph drawing
     * @param name name for graph, used for legend
     * @param line_style line style between data points for graph
     */
    QCP_Graph_Property( 
        const int id,
        const std::string &name, 
        const std::string &line_style, 
        const std::shared_ptr<QCP_Pen_Property> pen_property
    );

private: // internal variables
    int _id; // id for graph, that is, local index under the same axis
    
    QString _name; // name for graph, used for legend
    QCPGraph::LineStyle _line_style; // line style between data points for graph 

    std::shared_ptr<QCP_Pen_Property> _pen_property; // pen property for graph drawing

private: // internal methods
    void setName( const std::string &name );
    void setLineStyle( const std::string &line_style );

public: // accessors
    int Id() const { return _id; }
    
    QString Name() const { return _name; }
    QCPGraph::LineStyle LineStyle() const { return _line_style; }

    std::shared_ptr<QCP_Pen_Property> PenProperty() const { return _pen_property; }

}; // class QCP_Graph_Property

class QCP_Axis_Property {
public: // major methods
        
    /**
    * @brief Constructor for QCP Axis Property
    * 
    * @param id id for axis, used for local index under the same plot
    * @param graph_count number of graphs under the same axis
    * @param title title for axis
    * @param x_label x-axis label
    * @param y_label y-axis label
    * @param x_gap x-axis gap, for dynamic range
    * @param y_min y-axis minimum value
    * @param y_max y-axis maximum value
    * @param if_auto_scale if auto scale for y-axis
    * @param graph_properties graph properties under the same axis
    */
    QCP_Axis_Property( 
        const int id,
        const int graph_count,
        const std::string &title,
        const std::string &x_label,
        const std::string &y_label,
        const double x_gap,
        const double y_min,
        const double y_max,
        const bool if_auto_scale, 
        const std::vector< std::shared_ptr<QCP_Graph_Property> > &graph_properties
    );

private: // internal variables
    int _id; // id for axis, that is, local index under the same plot
    int _graph_count; // number of graphs under the same axis
    QString _title; // title for axis

    QString _x_label; // x-axis label
    QString _y_label; // y-axis label

    double _x_gap; // x-axis gap, for dynamic range
    double _y_min; // y-axis minimum value
    double _y_max; // y-axis maximum value
    bool _if_auto_scale; // if auto scale for y-axis

    std::vector< std::shared_ptr<QCP_Graph_Property> > _graph_properties; // graph properties under the same axis

private: // internal methods
    void setTitle( const std::string &title );
    void setXLabel( const std::string &x_label );
    void setYLabel( const std::string &y_label );

public: // accessors
    int Id() const { return _id; }
    int GraphCount() const { return _graph_count; }
    QString Title() const { return _title; }
    QString XLabel() const { return _x_label; }
    QString YLabel() const { return _y_label; }
    double XGap() const { return _x_gap; }
    double YMin() const { return _y_min; }
    double YMax() const { return _y_max; }
    bool IfAutoScale() const { return _if_auto_scale; }
    std::vector< std::shared_ptr<QCP_Graph_Property> > GraphProperties() const { return _graph_properties; }

}; // class QCP_Axis_Property

class QCP_Plot_Property {
public: // major methods
        
    /**
    * @brief Constructor for QCP Plot Property
    * 
    * @param id id for plot, used for local index under the same window
    * @param axis_count number of axes under the same plot
    * @param title title for plot
    */
    QCP_Plot_Property( 
        const int id,
        const int axis_count,
        const std::string &title, 
        const std::vector< std::shared_ptr<QCP_Axis_Property> > &axis_properties
    );

private: // internal variables
    int _id; // id for plot, that is, local index under the same window
    int _axis_count; // number of axes under the same plot

    std::string _title; // title for plot

    std::vector< std::shared_ptr<QCP_Axis_Property> > _axis_properties; // axis properties under the same plot

// private: // internal methods
//     void setTitle( const std::string &title );

public: // accessors
    int Id() const { return _id; }
    int AxisCount() const { return _axis_count; }
    std::string Title() const { return _title; }
    std::vector< std::shared_ptr<QCP_Axis_Property> > AxisProperties() const { return _axis_properties; }

}; // class QCP_Plot_Property

class QCP_Window_Property {
public: // major methods
            
        /**
        * @brief Constructor for QCP Window Property
        * 
        * @param id id for window, used for local index under the same monitor
        * @param channel message channel for window
        * @param plot_count number of plots under the same window
        * @param width window width, in pixels
        * @param height window height, in pixels
        * @param plot_properties plot properties under the same window
        */
        QCP_Window_Property( 
            const int id,
            const std::string &channel,
            const int plot_count,
            const int width,
            const int height,
            const std::vector< std::shared_ptr<QCP_Plot_Property> > &plot_properties
        );

private: // internal variables
    int _id; // id for window, that is, local index under the same monitor
    std::string _channel; // message channel for window
    int _plot_count; // number of plots under the same window

    int _width; // window width, in pixels
    int _height; // window height, in pixels

    std::vector< std::shared_ptr<QCP_Plot_Property> > _plot_properties; // plot properties under the same window

private: // internal methods

public: // accessors
    int Id() const { return _id; }
    std::string Channel() const { return _channel; }
    int PlotCount() const { return _plot_count; }
    int Width() const { return _width; }
    int Height() const { return _height; }
    std::vector< std::shared_ptr<QCP_Plot_Property> > PlotProperties() const { return _plot_properties; }

}; // class QCP_Window_Property

class QCP_Monitor_Property // : public arcs::sciq::Monitor_Abstraction::MonitorConfig 
{
public: // major methods
                
    /**
    * @brief Constructor for QCP Monitor Property
    * 
    * @param id id for monitor
    * @param window_count number of windows under the same monitor
    * @param window_properties window properties under the same monitor
    */
    QCP_Monitor_Property( 
        const int id,
        const int window_count,
        const std::vector< std::shared_ptr<QCP_Window_Property> > &window_properties
    );

private: // internal variables
    int _id; // id for monitor
    int _window_count; // number of windows under the same monitor

    std::vector< std::shared_ptr<QCP_Window_Property> > _window_properties; // window properties under the same monitor

private: // internal methods

public: // accessors
    int Id() const { return _id; }
    int WindowCount() const { return _window_count; }
    std::vector< std::shared_ptr<QCP_Window_Property> > WindowProperties() const { return _window_properties; }
    
}; // class QCP_Monitor_Property

/***************************/
/******** Qt Wrapper *******/
/***************************/

class QCP_Window;
class QCP_Plot;
class QCP_Axis;

class QCP_Window
{
public: // major methods

    QCP_Window( 
        const int id
    )
    : _id(id)
    {
        // window
        this->_window = std::make_shared<QMainWindow>();
        // central widget
        this->_centralWidget = std::make_shared<QWidget>( this->_window.get() );
        this->_layout = std::make_shared<QGridLayout>(this->_centralWidget.get());
    }

    ~QCP_Window() {}

private: // internal variables
    int _id; // id for window
    std::string _channel=""; // message channel for window

    std::shared_ptr<QMainWindow> _window; // window
    std::shared_ptr<QWidget> _centralWidget;
    std::shared_ptr<QGridLayout> _layout;

    std::vector< std::shared_ptr<QCP_Plot> > _plots; // child plots

public: // accessors
    int Id() { return _id; }

    void setChannel( const std::string &channel ) { this->_channel = channel; }
    std::string Channel() { return this->_channel; }

    std::shared_ptr<QMainWindow> Window() { return _window; }
    std::shared_ptr<QWidget> CentralWidget() { return _centralWidget; }
    std::shared_ptr<QGridLayout> Layout() { return _layout; }

    void insertPlot( std::shared_ptr<QCP_Plot> plot ) { this->_plots.push_back(plot); }
    std::vector< std::shared_ptr<QCP_Plot> > Plots() { return this->_plots; }

}; // class QCP_Window

class QCP_Plot
{
public: // major methods

    QCP_Plot( 
        const int id, 
        std::shared_ptr<QCP_Window> window
    ) 
    : _id(id), _parent_window(window) 
    {
        this->_plot = std::make_shared<QCustomPlot>( window->Window().get() );
        this->_axes.clear();
    }

    ~QCP_Plot() {}
private: // internal variables
    int _id; // id for plot
    std::string _title=""; // plot title
    std::string _channel=""; // message channel for plot
    std::shared_ptr<QCP_Window> _parent_window; 

    std::shared_ptr<QCustomPlot> _plot;
    std::shared_ptr<QCPTextElement> _title_element; 
    std::vector< std::shared_ptr<QCP_Axis> > _axes; // child axes

public: // accessors
    int Id() const { return _id; }

    void setTitle( const std::string &title ) { this->_title = title; }
    std::string Title() { return this->_title; }

    void setChannel( const std::string &channel ) { this->_channel = channel; }
    std::string Channel() { return this->_channel; }

    std::shared_ptr<QCP_Window> ParentWindow() { return _parent_window; }
    std::shared_ptr<QCustomPlot> Plot() { return _plot; }

    void setTitleElement( std::shared_ptr<QCPTextElement> title_element ) { this->_title_element = title_element; }
    std::shared_ptr<QCPTextElement> TitleElement() { return this->_title_element; }

    void insertAxis( std::shared_ptr<QCP_Axis> axis ) { this->_axes.push_back(axis); }
    std::vector< std::shared_ptr<QCP_Axis> > Axes() { return this->_axes; }

}; // class QCP_Plot

class QCP_Axis
{
public: // major methods

    QCP_Axis( 
        const int id, 
        std::shared_ptr<QCP_Plot> plot
    ) 
    : _id(id), _parent_plot(plot) 
    {
        this->_axis = std::make_shared<QCPAxisRect>( this->_parent_plot->Plot().get() );
        
        // clear
        this->_graph_id_index_map.clear();
        this->_legend_items.clear();
    }

    ~QCP_Axis() {}

private: // internal variables
    // general info
    int _id; // id for axis
    
    // connection
    std::shared_ptr<QCP_Plot> _parent_plot;
    std::shared_ptr<QCPAxisRect> _axis; // axis
    std::map<int,int> _graph_id_index_map; // map from [graph id] to [local index] under the same axis

    // legend
    std::shared_ptr<QCPLegend> _legend;
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > _legend_items;

    bool _if_auto_scale; // if auto scale for y-axis
    double _x_gap; // fixed x-axis gap, for dynamic range


public: // accessors
    int Id() { return _id; }
    std::shared_ptr<QCP_Plot> ParentPlot() { return _parent_plot; }
    std::shared_ptr<QCPAxisRect> Axis() { return _axis; }

    void insertGraphIdIndexMap( const int graph_id, const int index ) { 
        // check if graph id exists
        if ( this->_graph_id_index_map.find(graph_id) != this->_graph_id_index_map.end() ) {
            // if exists, update
            this->_graph_id_index_map[graph_id] = index; 
            return;
        }
        this->_graph_id_index_map[graph_id] = index; 
    }

    int getGraphIndex( const int graph_id ) { 
        // check if graph id exists
        if ( this->_graph_id_index_map.find(graph_id) != this->_graph_id_index_map.end() ) {
            // if exists, return
            return this->_graph_id_index_map[graph_id]; 
        }
        return -1;
    }

    std::string showGraphIdIndexMap() {
        std::stringstream ss;
        for ( const auto &pair : this->_graph_id_index_map ) {
            ss << "graph id [" << pair.first << "] index [" << pair.second << "]\n";
        }
        ss << "\n\n";
        return ss.str();
    }

    void setLegend( std::shared_ptr<QCPLegend> legend ) { this->_legend = legend; }
    std::shared_ptr<QCPLegend> Legend() { return this->_legend; }
    // std::shared_ptr<QCPLegend> &LegendRef() { return this->_legend; }

    void insertLegendItem( std::shared_ptr<QCPPlottableLegendItem> legend_item ) { this->_legend_items.push_back(legend_item); }
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > LegendItems() { return this->_legend_items; }

    void setAutoScale( const bool if_auto_scale ) { this->_if_auto_scale = if_auto_scale; }
    bool IfAutoScale() const { return _if_auto_scale; }
    
    void setXGap( const double x_gap ) { this->_x_gap = x_gap; }
    double XGap() const { return _x_gap; }
}; 



/**
 * @brief Real-time monitor class for multiple graphs, plots and windows based on QT5 and QCustomPlot. 
 * 
 */
class QCP_Monitor : public QObject
{
    // Q_OBJECT
public: // data structure
    using PenProperty = QCP_Pen_Property;
    using GraphProperty = QCP_Graph_Property; 
    using AxisProperty = QCP_Axis_Property;
    using PlotProperty = QCP_Plot_Property;
    using WindowProperty = QCP_Window_Property;

    enum Status {
        NORMAL,
        ERROR
    };

public: // major functions
    
    QCP_Monitor( 
        const std::shared_ptr<QCP_Monitor_Property> monitor_property
    );

    ~QCP_Monitor() {};

    /**
     * @brief Initialize the monitor with command line arguments. 
     * Multiple windows, plots, axes and graphs are created based on the monitor property. 
     * 
     * @param argc 
     * @param argv 
     * @return Status 
     */
    Status Initial( int argc, char *argv[] );

    /**
     * @brief Setup the monitor with signal-slot connections. 
     * It can be overrided for customized setup.
     * 
     * @return Status 
     */
    virtual Status Setup();

    /**
     * @brief Start the monitor with Qt application.
     * 
     * @return Status 
     */
    Status Start(); 

    virtual Status UpdateMonitor(); 

protected: // internal variables
    std::shared_ptr<QCP_Monitor_Property> _monitor_property;

    std::shared_ptr<QApplication> _app; // Qt application
    std::vector<std::shared_ptr<QCP_Window>> _windows; // collection of all Qt windows
    // std::vector<std::shared_ptr<QCP_Plot>> _plots; // collection of all Qt plots
    // std::vector<std::shared_ptr<QCP_Axis>> _axes; // collection of all Qt axes
    // std::vector<std::shared_ptr<QCP_Graph>> _graphs; // collection of all Qt graphs

    std::shared_ptr< QTimer > _dataTimer; 

protected: // internal methods

    std::shared_ptr<QCP_Window> CreateWindow( const std::shared_ptr<WindowProperty> window_property );

    std::shared_ptr<QCP_Plot> CreatePlot( const std::shared_ptr<PlotProperty> plot_property, std::shared_ptr<QCP_Window> window );

    std::shared_ptr<QCP_Axis> CreateAxis( const std::shared_ptr<AxisProperty> axis_property, std::shared_ptr<QCP_Plot> plot );

    // Status CreateGraph( const GraphProperty &graph_property, const std::shared_ptr<QCustomPlot> &plot );

}; // class QCP_Monitor


}//  namespace rt_display





