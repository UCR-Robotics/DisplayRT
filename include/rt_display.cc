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
 */

// Standard Includes
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>

#include "rt_display.h"

namespace rt_display {

/*****************************/
/******** Pen Property *******/
/*****************************/

QCP_Pen_Property::QCP_Pen_Property( 
    const std::string &color,
    const std::string &style,
    const int width
) 
{
    this->setColor(color);
    this->setStyle(style);
    this->setWidth(width);
}

void QCP_Pen_Property::setColor( const std::string &color )
{
    if (color == "black") {
        this->_color = Qt::GlobalColor::black;
    } else if (color == "white") {
        this->_color = Qt::GlobalColor::white;
    } else if (color == "gray") {
        this->_color = Qt::GlobalColor::gray;
    } else if (color == "red") {
        this->_color = Qt::GlobalColor::red;
    } else if (color == "green") {
        this->_color = Qt::GlobalColor::green;
    } else if (color == "blue") {
        this->_color = Qt::GlobalColor::blue;
    } else if (color == "cyan") {
        this->_color = Qt::GlobalColor::cyan;
    } else if (color == "magenta") {
        this->_color = Qt::GlobalColor::magenta;
    } else if (color == "yellow") {
        this->_color = Qt::GlobalColor::yellow;
    } else {
        this->_color = Qt::GlobalColor::black; // default color
        std::stringstream ss;
        ss << "\n\nQCP_Pen_Property: Invalid line color [" << color << "]\n\n";
        throw std::runtime_error(ss.str());
    }
}

void QCP_Pen_Property::setStyle( const std::string &style )
{
    if (style == "solid") {
        this->_style = Qt::PenStyle::SolidLine;
    } else if (style == "dash") {
        this->_style = Qt::PenStyle::DashLine;
    } else if (style == "dot") {
        this->_style = Qt::PenStyle::DotLine;
    } else if (style == "dashdot") {
        this->_style = Qt::PenStyle::DashDotLine;
    } else if (style == "dashdotdot") {
        this->_style = Qt::PenStyle::DashDotDotLine;
    } else {
        this->_style = Qt::PenStyle::SolidLine; // default style
        std::stringstream ss;
        ss << "\n\nQCP_Pen_Property: Invalid line style [" << style << "]\n\n";
        throw std::runtime_error(ss.str());
    }
}

void QCP_Pen_Property::setWidth( const int width )
{
    this->_width = width;
}

/*******************************/
/******** Graph Property *******/
/*******************************/

QCP_Graph_Property::QCP_Graph_Property( 
    const int id,
    const std::string &name, 
    const std::string &line_style, 
    const std::shared_ptr<QCP_Pen_Property> pen_property
) : _id(id), _pen_property(pen_property)
{
    // this->_pen_property = pen_property;
    this->setName(name);
    this->setLineStyle(line_style);
}

void QCP_Graph_Property::setName( const std::string &name )
{
    _name = QString::fromStdString(name);
}

void QCP_Graph_Property::setLineStyle( const std::string &line_style )
{
    
    if (line_style == "none") {
        this->_line_style = QCPGraph::LineStyle::lsNone;
    } else if (line_style == "line") {
        this->_line_style = QCPGraph::LineStyle::lsLine;
    } else if (line_style == "step_center") {
        this->_line_style = QCPGraph::LineStyle::lsStepCenter;
    } else if (line_style == "step_left") {
        this->_line_style = QCPGraph::LineStyle::lsStepLeft;
    } else if (line_style == "step_right") {
        this->_line_style = QCPGraph::LineStyle::lsStepRight;
    } else if (line_style == "impulse") {
        this->_line_style = QCPGraph::LineStyle::lsImpulse;
    }  
    else {
        this->_line_style = QCPGraph::LineStyle::lsNone; // default style
        std::stringstream ss;
        ss << "\n\nQCP_Graph_Property: Invalid line style [" << line_style << "]\n\n";
        throw std::runtime_error(ss.str());
    }
}

/******************************/
/******** Axis Property *******/
/******************************/

QCP_Axis_Property::QCP_Axis_Property( 
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
) 
: _id(id), _graph_count(graph_count), 
  _x_gap(x_gap), _y_min(y_min), _y_max(y_max), 
  _if_auto_scale(if_auto_scale), 
  _graph_properties(graph_properties)
{
    this->setTitle(title);
    this->setXLabel(x_label);
    this->setYLabel(y_label);
}

void QCP_Axis_Property::setTitle( const std::string &title )
{
    this->_title = QString::fromStdString(title);
}

void QCP_Axis_Property::setXLabel( const std::string &x_label )
{
    this->_x_label = QString::fromStdString(x_label);
}

void QCP_Axis_Property::setYLabel( const std::string &y_label )
{
    this->_y_label = QString::fromStdString(y_label);
}

/******************************/
/******** Plot Property *******/
/******************************/

QCP_Plot_Property::QCP_Plot_Property( 
    const int id,
    const int axis_count,
    const std::string &title, 
    const std::vector< std::shared_ptr<QCP_Axis_Property> > &axis_properties
) : _id(id), _axis_count(axis_count), _title(title), _axis_properties(axis_properties)
{
    // this->setTitle(title);
}

// void QCP_Plot_Property::setTitle( const std::string &title )
// {
//     this->_title = QString::fromStdString(title);
// }

/********************************/
/******** Window Property *******/
/********************************/

QCP_Window_Property::QCP_Window_Property( 
    const int id,
    const std::string &channel,
    const int plot_count,
    const int width,
    const int height,
    const std::vector< std::shared_ptr<QCP_Plot_Property> > &plot_properties
) 
: _id(id), _channel(channel), _plot_count(plot_count), 
  _width(width), _height(height), 
  _plot_properties(plot_properties)
{
}

/*********************************/
/******** Monitor Property *******/
/*********************************/

QCP_Monitor_Property::QCP_Monitor_Property( 
    const int id,
    const int window_count,
    const std::vector< std::shared_ptr<QCP_Window_Property>> &window_properties
)
: // MonitorConfig(id), 
  _id(id), _window_count(window_count), _window_properties(window_properties)
{
}

/***************************/
/******** QCP Window *******/
/***************************/

// QCP_Window::QCP_Window( 
//     const int id
// )
// : QMainWindow(), _id(id)
// {}

/**********************************/
/******** Monitor Interface *******/
/**********************************/

QCP_Monitor::QCP_Monitor( 
    const std::shared_ptr<QCP_Monitor_Property> monitor_property 
)
: _monitor_property(monitor_property)
{
}

QCP_Monitor::Status QCP_Monitor::Initial( int argc, char *argv[] )
{
    { // debug
        std::cout << "\nQCP_Monitor: Initialization starts\n";
    }
    
    // safety check
    if ( this->_monitor_property->WindowProperties().size() != this->_monitor_property->WindowCount() ) {
        std::stringstream ss;
        ss << "\n\nQCP_Monitor: invalid window count [" << this->_monitor_property->WindowCount() << "]\n\n";
        throw std::runtime_error(ss.str());
        return Status::ERROR;
    }
    
    this->_app = std::make_shared<QApplication>(argc, argv);

    // setup windows
    for ( int i = 0; i < this->_monitor_property->WindowCount(); i++ ) 
    {
        const auto window_property = this->_monitor_property->WindowProperties()[i];
        auto window = this->CreateWindow( window_property );
        if ( window == nullptr ) {
            std::stringstream ss;
            ss << "\n\nQCP_Monitor: window " << window_property->Id() << " failed to create\n\n";
            throw std::runtime_error(ss.str());
            return Status::ERROR;
        } 

        this->_windows.push_back( window );
        window->Window()->show();
    }

    // QTimer *dataTimer = new QTimer( this->_app.get() );
    // for ( auto window : this->_windows )
    // {
    //     QObject::connect(dataTimer, &QTimer::timeout, [window]() {
    //         static double t = 0;
    //         t += 0.1;

    //         for ( auto &plot : window->Plots() ) 
    //         {
    //             for ( int i = 0; i < plot->Axes().size(); i++ )             
    //             {
    //                 auto axis = plot->Axes()[i];
    //                 const auto xgap = axis->XGap();

    //                 for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
    //                 {
    //                     axis->Axis()->graphs()[j]->addData(t, std::sin( t + static_cast<double>( M_PI_4 * j ) ) );
    //                     if ( (t-xgap) > 0.0 )
    //                         axis->Axis()->graphs()[j]->data()->removeBefore(t-xgap);
    //                 }

    //                 if ( (t-xgap) > 0.0 )
    //                     axis->Axis()->axis(QCPAxis::atBottom)->setRange(t - xgap, t);
    //             }
                
    //             plot->Plot()->replot();
    //         }
    //     });
    // }
    // dataTimer->start(500); // Update every 500 ms

    { // debug
        std::cout << "\nQCP_Monitor: Initialization finishes\n";
    }

    return Status::NORMAL; 
}

QCP_Monitor::Status QCP_Monitor::Setup()
{
    { // debug
        std::cout << "\nQCP_Monitor: Setup starts\n";
    }

    QTimer *dataTimer = new QTimer( this->_app.get() );
    // for ( auto window : this->_windows )
    // {
    //     QObject::connect(dataTimer, &QTimer::timeout, [window]() {
    //         static double t = 0;
    //         t += 0.1;

    //         for ( auto &plot : window->Plots() ) 
    //         {
    //             for ( int i = 0; i < plot->Axes().size(); i++ )             
    //             {
    //                 auto axis = plot->Axes()[i];
    //                 const auto xgap = axis->XGap();

    //                 for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
    //                 {
    //                     axis->Axis()->graphs()[j]->addData(t, std::sin( t + static_cast<double>( M_PI_4 * j ) ) );
    //                     if ( (t-xgap) > 0.0 )
    //                         axis->Axis()->graphs()[j]->data()->removeBefore(t-xgap);
    //                 }

    //                 if ( (t-xgap) > 0.0 )
    //                     axis->Axis()->axis(QCPAxis::atBottom)->setRange(t - xgap, t);
    //             }
                
    //             plot->Plot()->replot();
    //         }
    //     });
    // }
    // QObject::connect(dataTimer, &QTimer::timeout, [this]() {
    //     for ( auto window : this->_windows )
    //     {
    //         static double t = 0;
    //         t += 0.1;

    //         for ( auto &plot : window->Plots() ) 
    //         {
    //             for ( int i = 0; i < plot->Axes().size(); i++ )             
    //             {
    //                 auto axis = plot->Axes()[i];
    //                 const auto xgap = axis->XGap();

    //                 for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
    //                 {
    //                     axis->Axis()->graphs()[j]->addData(t, std::sin( t + static_cast<double>( M_PI_4 * j ) ) );
    //                     if ( (t-xgap) > 0.0 )
    //                         axis->Axis()->graphs()[j]->data()->removeBefore(t-xgap);
    //                 }

    //                 if ( (t-xgap) > 0.0 )
    //                     axis->Axis()->axis(QCPAxis::atBottom)->setRange(t - xgap, t);
    //             }
                
    //             plot->Plot()->replot();
    //         }
    //     }
    // } ); 
    QObject::connect( dataTimer, &QTimer::timeout, this, &QCP_Monitor::UpdateMonitor );
    // QObject::connect( dataTimer, SIGNAL(QTimer::timeout()), this, SLOT(QCP_Monitor::UpdateMonitor()) );
    dataTimer->start(100); // Update every 100 ms
    
    { // debug
        std::cout << "\nQCP_Monitor: Setup finishes\n";
    }

    return Status::NORMAL; 
}

QCP_Monitor::Status QCP_Monitor::Start() 
{ 
    // { // debug
    //     std::cout << "\nQCP_Monitor: Start windows display\n";
    // }
    
    // for ( auto &window : this->_windows )
    // {
    //     window->Window()->show();
    // }

    { // debug
        std::cout << "\nQCP_Monitor: Start QT application\n";
    }
    
    this->_app->exec(); 
    
    return Status::NORMAL; 
}

QCP_Monitor::Status QCP_Monitor::UpdateMonitor()
{
    for ( auto window : this->_windows )
    {
        static double t = 0;
        t += 0.1;

        for ( auto &plot : window->Plots() ) 
        {
            // load data
            for ( int i = 0; i < plot->Axes().size(); i++ )             
            {
                auto axis = plot->Axes()[i];
                // const auto xgap = axis->XGap();

                for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
                {
                    axis->Axis()->graphs()[j]->addData(t, std::sin( t + static_cast<double>( M_PI / 12.0 * j ) ) );
                } 
            }

            // adjust axis features
            for ( int i = 0; i < plot->Axes().size(); i++ )             
            {
                auto axis = plot->Axes()[i];
                const auto xgap = axis->XGap();

                for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
                {
                    if ( (t-xgap) > 0.0 )
                        axis->Axis()->graphs()[j]->data()->removeBefore(t-xgap);
                }

                // x-axis range
                if ( (t-xgap) > 0.0 )
                    axis->Axis()->axis(QCPAxis::atBottom)->setRange(t - xgap, t);
                
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

    return Status::NORMAL;
}

std::shared_ptr<QCP_Window> QCP_Monitor::CreateWindow( 
    const std::shared_ptr<WindowProperty> window_property 
)
{
    { // debug
        #if DEBUG_LEVEL > 0
        std::cout << "\nQCP_Monitor: Create window " << window_property->Id() << " starts\n";
        #endif
    }
    
    // safety check
    if ( window_property->PlotProperties().size() != window_property->PlotCount() ) {
        std::stringstream ss;
        ss << "\n\nQCP_Monitor: window " << window_property->Id() << " has invalid plot count [" << window_property->PlotCount() << "]\n\n";
        throw std::runtime_error(ss.str());
        return nullptr;
    }
    
    std::shared_ptr<QCP_Window> window = std::make_shared<QCP_Window>(window_property->Id());
    window->setChannel(window_property->Channel());
    // std::shared_ptr<QWidget> centralWidget = std::make_shared<QWidget>(window.get());
    // std::shared_ptr<QGridLayout> layout = std::make_shared<QGridLayout>(centralWidget.get());
    
    // setup plots
    for ( int i = 0; i < window_property->PlotCount(); i++ ) 
    {
        const auto plot_property = window_property->PlotProperties()[i];
        auto plot = this->CreatePlot( plot_property, window );
        if ( plot == nullptr ) 
        {
            std::stringstream ss;
            ss << "\n\nQCP_Monitor: window " << window_property->Id() << " plot " << plot_property->Id() << " failed to create\n\n";
            throw std::runtime_error(ss.str());
            return nullptr;
        } 
                
        window->Layout()->addWidget( plot->Plot().get(), i, 0 );
        window->insertPlot( plot );
    }

    // setup window
    window->CentralWidget()->setLayout(window->Layout().get());
    window->Window()->setCentralWidget(window->CentralWidget().get());
    window->Window()->resize(window_property->Width(), window_property->Height());

    // // test: signal
    // QTimer *dataTimer = new QTimer(window->Window().get());
    // QObject::connect(dataTimer, &QTimer::timeout, [window]() {
    //     static double t = 0;
    //     t += 0.1;

    //     for ( auto &plot : window->Plots() ) 
    //     {
    //         for ( int i = 0; i < plot->Axes().size(); i++ )             
    //         {
    //             auto axis = plot->Axes()[i];
    //             const auto xgap = axis->XGap();

    //             for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
    //             {
    //                 axis->Axis()->graphs()[j]->addData(t, std::sin( t + static_cast<double>( M_PI_4 * j ) ) );
    //                 if ( (t-xgap) > 0.0 )
    //                     axis->Axis()->graphs()[j]->data()->removeBefore(t-xgap);
    //             }

    //             if ( (t-xgap) > 0.0 )
    //                 axis->Axis()->axis(QCPAxis::atBottom)->setRange(t - xgap, t);
    //         }
            
    //         plot->Plot()->replot();
    //     }
    // });
    // dataTimer->start(500); // Update every 500 ms

    { // debug
        #if DEBUG_LEVEL > 0
        std::cout << "\nQCP_Monitor: Create window " << window_property->Id() << " finishes\n";
        #endif
    }

    return window; 
}

std::shared_ptr<QCP_Plot> QCP_Monitor::CreatePlot( 
    const std::shared_ptr<QCP_Monitor::PlotProperty> plot_property, 
    std::shared_ptr<QCP_Window> window 
)
{
    { // debug
        #if DEBUG_LEVEL > 0
        std::cout << "\nQCP_Monitor: Create plot " << plot_property->Id() << " starts\n";
        #endif
    }

    // safety check
    if ( plot_property->AxisProperties().size() != plot_property->AxisCount() ) {
        std::stringstream ss;
        ss << "\n\nQCP_Monitor: window " << window->Id() << " plot " << plot_property->Id() << " has invalid axis count [" << plot_property->AxisCount() << "]\n\n";
        throw std::runtime_error(ss.str());
        return nullptr;
    }
    
    std::shared_ptr<QCP_Plot> plot = std::make_shared<QCP_Plot>( plot_property->Id(), window );
    plot->Plot()->plotLayout()->clear();
    plot->setTitle(plot_property->Title());
    plot->setChannel(window->Channel());

    // setup plot title
    plot->Plot()->plotLayout()->insertRow(0);
    plot->setTitleElement( std::make_shared<QCPTextElement>( 
        plot->Plot().get(), 
        QString::fromStdString( window->Channel().append(":").append( plot->Title() ) ), 
        QFont("sans", 12, QFont::Bold)) 
    );
    plot->Plot()->plotLayout()->addElement(0, 0, plot->TitleElement().get() );

    // setup axes
    for ( int i = 0; i < plot_property->AxisCount(); i++ ) 
    {
        const auto axis_property = plot_property->AxisProperties()[i];
        auto axis = this->CreateAxis( axis_property, plot );
        if ( axis == nullptr ) {
            std::stringstream ss;
            ss << "\n\nQCP_Monitor: window " << window->Id() << " plot " << plot_property->Id() << " axis " << axis_property->Id() << " failed to create\n\n";
            throw std::runtime_error(ss.str());
            return nullptr;
        } 

        plot->Plot()->plotLayout()->addElement( i+1, 0, axis->Axis().get() ); // first row is for title
        plot->insertAxis( axis );
    }

    // setup legends
    for ( int i = 0; i < plot_property->AxisCount(); i++ ) 
    {
        auto axis = plot->Axes()[i];
        const auto axis_property = plot_property->AxisProperties()[i];

        // legend
        axis->setLegend( std::make_shared<QCPLegend>() );
        axis->Axis()->insetLayout()->addElement( axis->Legend().get() , Qt::AlignTop | Qt::AlignRight );
        axis->Legend()->setVisible(true);
        axis->Legend()->setBrush(QBrush(QColor(255, 255, 255, 150)));
        axis->Legend()->setBorderPen(QPen(Qt::black));

        for ( int j = 0; j < axis_property->GraphCount(); j++ ) 
        {
            axis->insertLegendItem( std::make_shared<QCPPlottableLegendItem>( axis->Legend().get(), axis->Axis()->graphs()[j] ) ); 
            axis->Legend()->addItem( axis->LegendItems().back().get() );
        }
    }

    // // test: signal
    // QTimer *dataTimer = new QTimer(plot->Plot().get());
    // QObject::connect(dataTimer, &QTimer::timeout, [plot]() {
    //     static double t = 0;
    //     t += 0.1;

    //     // { //debug
    //     //     std::cout << "plot " << plot->Id() << " axis count = " << plot->Axes().size() << "\n";
    //     // }
    //     for ( int i = 0; i < plot->Axes().size(); i++ ) 
    //     {
    //         // { // debug
    //         //     std::cout << "plot " << plot->Id() << " axis " << i << " graph count = " << plot->Axes()[i]->Axis()->graphs().size() << "\n";
    //         // }
            
    //         auto axis = plot->Axes()[i];
    //         const auto xgap = axis->XGap();

    //         for ( int j = 0; j < axis->Axis()->graphs().size(); j++ ) 
    //         {
    //             axis->Axis()->graphs()[j]->addData(t, std::sin( t + static_cast<double>( M_PI_4 * j ) ) );
    //             // if ( (t-xgap) > 0.0 )
    //                 axis->Axis()->graphs()[j]->data()->removeBefore(t-xgap);

    //             { // debug
    //                 std::cout << "window " << plot->ParentWindow()->Id() << " plot " << plot->Id() << " axis " << i << " graph " << j << " replotted\n"; 
    //                         //   << "t = " << t << ", xgap = " << xgap << "\n";
    //             }
    //         }

    //         // if ( (t-xgap) > 0.0 )
    //             axis->Axis()->axis(QCPAxis::atBottom)->setRange(t - xgap, t);
            
    //         { //debug
    //             std::cout << "window " << plot->ParentWindow()->Id() << " plot " << plot->Id() << " axis " << i << " replotted: "
    //                       << "t = " << t << ", xgap = " << xgap << "\n";
    //         }
    //     }

    //     plot->Plot()->replot();

    //     { // debug
    //         std::cout << "window " << plot->ParentWindow()->Id() << " plot " << plot->Id() << " replotted\n\n";
    //     }
    // });
    // dataTimer->start(500); // Update every 20 ms

    { // debug
        #if DEBUG_LEVEL > 0
        std::cout << "\nQCP_Monitor: Create plot " << plot_property->Id() << " finishes\n";
        #endif
    }

    return plot; 
}

std::shared_ptr<QCP_Axis> QCP_Monitor::CreateAxis( 
    const std::shared_ptr<QCP_Monitor::AxisProperty> axis_property, 
    std::shared_ptr<QCP_Plot> plot 
)
{
    { // debug
        #if DEBUG_LEVEL > 0
        std::cout << "\nQCP_Monitor: Create axis " << axis_property->Id() << " starts\n";
        #endif
    }

    // safety check
    if ( axis_property->GraphProperties().size() != axis_property->GraphCount() ) {
        std::stringstream ss;
        ss << "\n\nQCP_Monitor: window " << plot->ParentWindow()->Id() << " plot " << plot->Id() << " axis " << axis_property->Id() << " has invalid graph count [" << axis_property->GraphCount() << "]\n\n";
        throw std::runtime_error(ss.str());
        return nullptr;
    }
    
    std::shared_ptr<QCP_Axis> axis = std::make_shared<QCP_Axis>( axis_property->Id(), plot );

    // labels
    axis->Axis()->axis(QCPAxis::atBottom)->setLabel( axis_property->XLabel() );
    axis->Axis()->axis(QCPAxis::atLeft)->setLabel( axis_property->YLabel() );
    // range
    axis->Axis()->axis(QCPAxis::atBottom)->setRange( 0, axis_property->XGap() );
    if ( axis_property->IfAutoScale() ) {
        axis->Axis()->axis(QCPAxis::atLeft)->rescale(true); // auto scale
        axis->setAutoScale(true);
    } else if ( (!std::isnan( axis_property->YMin() )) && (!std::isnan( axis_property->YMax() )) ) {
        axis->Axis()->axis(QCPAxis::atLeft)->setRange( axis_property->YMin(), axis_property->YMax() );
        axis->setAutoScale(false);
    }
    else {
        std::stringstream ss;
        ss << "\n\nQCP_Monitor: window " << plot->ParentWindow()->Id() << " plot " << plot->Id() << " axis " << axis_property->Id() << " has invalid y-axis range\n\n";
        throw std::runtime_error(ss.str());
        return nullptr;
    }
    // axis->Axis()->axis(QCPAxis::atLeft)->setRange( axis_property->YMin(), axis_property->YMax() );
    // axis->Axis()->axis(QCPAxis::atLeft)->rescale(true); // auto scale
    axis->setXGap( axis_property->XGap() );
    
    // // legend
    // axis->setLegend( std::make_shared<QCPLegend>() );
    // axis->Axis()->insetLayout()->addElement( axis->Legend().get() , Qt::AlignTop | Qt::AlignRight );
    // axis->Legend()->setVisible(true);
    // axis->Legend()->setBrush(QBrush(QColor(255, 255, 255, 150)));
    // axis->Legend()->setBorderPen(QPen(Qt::black));


    // setup graphs
    for ( int i = 0; i < axis_property->GraphCount(); i++ ) 
    {
        const auto graph_property = axis_property->GraphProperties()[i];
        
        // pen
        QPen pen;
        pen.setColor( graph_property->PenProperty()->Color() );
        pen.setStyle( graph_property->PenProperty()->Style() );
        pen.setWidth( graph_property->PenProperty()->Width() );

        // graph
        plot->Plot()->addGraph( axis->Axis()->axis(QCPAxis::atBottom), axis->Axis()->axis(QCPAxis::atLeft) );

        axis->Axis()->graphs().back()->setPen(pen);
        axis->Axis()->graphs().back()->setLineStyle( graph_property->LineStyle() );
        axis->Axis()->graphs().back()->setName( graph_property->Name() );

        axis->insertGraphIdIndexMap( graph_property->Id(), i );

        // legend item
        
        // { // debug
        //     std::cout << "before add legend item " << i <<", legend item count from legend = " << axis->Legend()->itemCount() << "\n";
        // }
        
        // axis->Legend()->addItem( std::make_shared<QCPPlottableLegendItem>( axis->Legend().get(), axis->Axis()->graphs().back() ).get() ); 
        // axis->insertLegendItem( std::make_shared<QCPPlottableLegendItem>( axis->Legend().get(), axis->Axis()->graphs().back() ) ); 
        // axis->Legend()->addItem( axis->LegendItems().back().get() );
        
        // { // debug
        //     std::cout << "after add legend item " << i <<", legend item count from legend = " << axis->Legend()->itemCount() << "\n";
        // }

        // // test: data
        // QTimer *dataTimer = new QTimer(plot->Plot().get());
        // QObject::connect(dataTimer, &QTimer::timeout, [&plot, &axis, &axis_property]() {
        //     const auto xgap = axis_property->XGap();
            
        //     static double t = 0;
        //     t += 0.1;

        //     axis->graphs().back()->addData(t, qSin(t));
        //     axis->graphs().back()->data()->removeBefore(t-xgap);

        //     plot->replot();
        // });
        // dataTimer->start(20); // Update every 20 ms
    }

    // // legend
    // axis->setLegend( std::make_shared<QCPLegend>() );
    // axis->Axis()->insetLayout()->addElement( axis->Legend().get() , Qt::AlignTop | Qt::AlignRight );
    // axis->Legend()->setVisible(true);
    // axis->Legend()->setBrush(QBrush(QColor(255, 255, 255, 150)));
    // axis->Legend()->setBorderPen(QPen(Qt::black));

    // for ( int i = 0; i < axis_property->GraphCount(); i++ ) 
    // {
    //     axis->insertLegendItem( std::make_shared<QCPPlottableLegendItem>( axis->Legend().get(), axis->Axis()->graphs()[i] ) ); 
    //     axis->Legend()->addItem( axis->LegendItems().back().get() );
    // }


    // { // debug
    //     std::stringstream ss;
    //     ss << "\n\nQCP_Monitor: window " << plot->ParentWindow()->Id() << " plot " << plot->Id() << " axis " << axis->Id() << " created\n";
    //     ss << "graph count = " << axis->Axis()->graphs().size() << "\n";
    //     ss << "graph count from plot = " << plot->Plot()->graphCount() << "\n";
    //     // ss << "legend item count = " << static_cast<int>( axis->LegendItems().size() ) << "\n";
    //     // ss << "legend item count from legend = " << axis->_legend->itemCount() << "\n";
    //     ss << "legend item count from legend = " << axis->Legend()->itemCount() << "\n";
    //     ss << "legend item count from plot = " << plot->Plot()->legend->itemCount() << "\n";
    //     ss << "graph ID-Index map = \n" << axis->showGraphIdIndexMap();
        
    //     int legendCount = 0;
    //     { // count legends
    //         auto insetLayout = axis->Axis()->insetLayout();
    //         for (int i = 0; i < insetLayout->elementCount(); ++i) {
    //             if (qobject_cast<QCPLegend *>(insetLayout->elementAt(i))) {
    //                 ++legendCount;
    //             }
    //         }
    //     }
    //     ss << "legend count = " << legendCount << "\n";
        
        
    //     std::cout << ss.str();
    // }

    { // debug
        #if DEBUG_LEVEL > 0
        std::cout << "\nQCP_Monitor: Create axis " << axis_property->Id() << " finishes\n";
        #endif
    }

    return axis; 
}


}//  namespace rt_display





