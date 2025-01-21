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

#include <memory>
#include <iostream>
#include <sstream>
#include <string>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>

// #ifndef QCUSTOMPLOT_USE_LIBRARY
// #define QCUSTOMPLOT_USE_LIBRARY
// #endif

#include "qcustomplot.h"

void setupPlot( 
    std::shared_ptr<QMainWindow> window, 
    std::shared_ptr<QCustomPlot> &customPlot, 
    std::shared_ptr<QCPAxisRect> &topPlot,
    std::shared_ptr<QCPAxisRect> &bottomPlot,
    std::shared_ptr<QCPLegend> &topLegend,
    std::shared_ptr<QCPLegend> &bottomLegend,
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > &topLegendItems,
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > &bottomLegendItems
)
{
    customPlot = std::make_shared<QCustomPlot>(window.get());

    
    // Create axes
    topPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
    bottomPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
    // std::shared_ptr< QCPAxisRect> topPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
    // std::shared_ptr< QCPAxisRect> bottomPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
    
    customPlot->plotLayout()->clear(); // Clear the default axis rect
    customPlot->plotLayout()->addElement(0, 0, topPlot.get());
    customPlot->plotLayout()->addElement(1, 0, bottomPlot.get());

    // axis legends
    // auto topLegend = std::make_shared<QCPLegend>();
    // auto bottomLegend = std::make_shared<QCPLegend>();
    // std::vector< std::shared_ptr<QCPPlottableLegendItem> > topLegendItems;
    // std::vector< std::shared_ptr<QCPPlottableLegendItem> > bottomLegendItems;
    topLegend = std::make_shared<QCPLegend>();
    bottomLegend = std::make_shared<QCPLegend>();
    topLegendItems.clear();
    bottomLegendItems.clear();

    // axis labels
    topPlot->axis(QCPAxis::atBottom)->setLabel("Time [s]");
    topPlot->axis(QCPAxis::atLeft)->setLabel("Amplitude");

    bottomPlot->axis(QCPAxis::atBottom)->setLabel("Time [s]");
    bottomPlot->axis(QCPAxis::atLeft)->setLabel("Amplitude");

    // axis range
    topPlot->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);
    bottomPlot->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);

    // Create graphs in the top subplot
    QPen pen;
    customPlot->addGraph(topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft));
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    topPlot->graphs().back()->setPen(pen);
    topPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    topPlot->graphs().back()->setName("Sine");

    customPlot->addGraph(topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft));
    pen.setColor(Qt::red);
    pen.setWidth(4);
    pen.setStyle(Qt::DashLine);
    topPlot->graphs().back()->setPen(pen);
    topPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    topPlot->graphs().back()->setName("Cosine");

    // Create graphs in the bottom subplot
    customPlot->addGraph(bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft));
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    bottomPlot->graphs().back()->setPen(pen);
    bottomPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    bottomPlot->graphs().back()->setName("Tangent");

    customPlot->addGraph(bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft));
    pen.setColor(Qt::magenta);
    pen.setWidth(4);
    pen.setStyle(Qt::DashLine);
    bottomPlot->graphs().back()->setPen(pen);
    bottomPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    bottomPlot->graphs().back()->setName("Cotangent");

    // legends
    // auto topLegend = std::make_shared<QCPLegend>();
    topPlot->insetLayout()->addElement(topLegend.get(), Qt::AlignTop | Qt::AlignRight);
    topLegend->setVisible(true);
    topLegend->setBrush(QBrush(QColor(255, 255, 255, 150)));
    topLegend->setBorderPen(QPen(Qt::black));

    // auto bottomLegend = std::make_shared<QCPLegend>();
    bottomPlot->insetLayout()->addElement(bottomLegend.get(), Qt::AlignTop | Qt::AlignRight);
    bottomLegend->setVisible(true);
    bottomLegend->setBrush(QBrush(QColor(255, 255, 255, 150)));
    bottomLegend->setBorderPen(QPen(Qt::black));

    // Add graphs to the legends
    topLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[0]) );
    topLegend->addItem( topLegendItems.back().get() );
    topLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[1]) );
    topLegend->addItem( topLegendItems.back().get() );

    bottomLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[0]) );
    bottomLegend->addItem( bottomLegendItems.back().get() );
    bottomLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[1]) );
    bottomLegend->addItem( bottomLegendItems.back().get() );

    // Setup a timer to update the plot
    QTimer *dataTimer = new QTimer(customPlot.get());
    QObject::connect(dataTimer, &QTimer::timeout, [customPlot, topPlot, bottomPlot]() {
        static double t = 0;
        t += 0.1;
        customPlot->graph(0)->addData(t, qSin(t));
        customPlot->graph(0)->data()->removeBefore(t - 5);
        customPlot->graph(1)->addData(t, qCos(t));
        customPlot->graph(1)->data()->removeBefore(t - 5);
        topPlot->axis(QCPAxis::atBottom)->setRange(t - 5, t);

        customPlot->graph(2)->addData(t, qSin(2.0 * t));
        customPlot->graph(2)->data()->removeBefore(t - 5);
        customPlot->graph(3)->addData(t, qCos(2.0 * t));
        customPlot->graph(3)->data()->removeBefore(t - 5);
        bottomPlot->axis(QCPAxis::atBottom)->setRange(t - 5, t);

        customPlot->replot();
    });
    dataTimer->start(100); // Update every 100 ms

    { // debug
        std::stringstream ss;
        ss << "graph count = " << topPlot->graphs().size() << "\n";
        ss << "graph count from plot = " << customPlot->graphCount() << "\n";
        // ss << "legend item count = " << static_cast<int>( axis->LegendItems().size() ) << "\n";
        // ss << "legend item count from legend = " << topPlot->_legend->itemCount() << "\n";
        ss << "legend item count from plot = " << customPlot->legend->itemCount() << "\n";
        // ss << "graph ID-Index map = \n" << axis->showGraphIdIndexMap();

        std::cout << ss.str() << std::endl;

    }
}

class MyWindow : public QMainWindow
{
public: 
    std::shared_ptr<QWidget> centralWidget;
    std::shared_ptr<QGridLayout> layout;
    
    std::shared_ptr<QCustomPlot> customPlot1; 
    std::shared_ptr<QCPAxisRect> topPlot1;
    std::shared_ptr<QCPAxisRect> bottomPlot1; 
    std::shared_ptr<QCPLegend> topLegend1;
    std::shared_ptr<QCPLegend> bottomLegend1; 
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > topLegendItems1;
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > bottomLegendItems1;

    std::shared_ptr<QCustomPlot> customPlot2;
    std::shared_ptr<QCPAxisRect> topPlot2;
    std::shared_ptr<QCPAxisRect> bottomPlot2;
    std::shared_ptr<QCPLegend> topLegend2;
    std::shared_ptr<QCPLegend> bottomLegend2;
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > topLegendItems2;
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > bottomLegendItems2;
};

void setupWindow( std::shared_ptr<MyWindow> window, const int px, const int py, const int width, const int height )
{
    // Create a central widget and a layout
    // auto centralWidget = std::make_shared<QWidget>( window.get() );
    // auto layout = std::make_shared<QGridLayout>( centralWidget.get() );
    window->centralWidget = std::make_shared<QWidget>( window.get() );
    window->layout = std::make_shared<QGridLayout>( window->centralWidget.get() );

    setupPlot(window, window->customPlot1, window->topPlot1, window->bottomPlot1, window->topLegend1, window->bottomLegend1, window->topLegendItems1, window->bottomLegendItems1);
    setupPlot(window, window->customPlot2, window->topPlot2, window->bottomPlot2, window->topLegend2, window->bottomLegend2, window->topLegendItems2, window->bottomLegendItems2);
    { // debug
        std::cout << "setup plots done" << std::endl;
    }

    window->layout->addWidget(window->customPlot1.get(),0,0);
    window->layout->addWidget(window->customPlot2.get(),1,0);
    { // debug
        std::cout << "add plots to layout done" << std::endl;
    }

    window->centralWidget->setLayout(window->layout.get());
    { // debug
        std::cout << "set layout to central widget done" << std::endl;
    }

    window->setCentralWidget(window->centralWidget.get());
    { // debug
        std::cout << "set central widget to window done" << std::endl;
    }

    // window->resize(800, 600);
    window->setGeometry(px, py, width, height); 
    { // debug
        std::cout << "resize window done" << std::endl;
    }

    return;

}




// void setupPlot2( 
//     std::shared_ptr<QMainWindow> window, 
//     std::shared_ptr<QCustomPlot> &customPlot, 
//     std::shared_ptr<QCPAxisRect> &topPlot,
//     std::shared_ptr<QCPAxisRect> &bottomPlot,
//     std::shared_ptr<QCPLegend> &topLegend,
//     std::shared_ptr<QCPLegend> &bottomLegend,
//     std::vector< std::shared_ptr<QCPPlottableLegendItem> > &topLegendItems,
//     std::vector< std::shared_ptr<QCPPlottableLegendItem> > &bottomLegendItems
// )
// {
//     customPlot = std::make_shared<QCustomPlot>(window.get());

    
//     // Create axes
//     topPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
//     bottomPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
//     // std::shared_ptr< QCPAxisRect> topPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
//     // std::shared_ptr< QCPAxisRect> bottomPlot = std::make_shared<QCPAxisRect>( customPlot.get() );
    
//     customPlot->plotLayout()->clear(); // Clear the default axis rect
//     customPlot->plotLayout()->addElement(0, 0, topPlot.get());
//     customPlot->plotLayout()->addElement(1, 0, bottomPlot.get());

//     // axis legends
//     // auto topLegend = std::make_shared<QCPLegend>();
//     // auto bottomLegend = std::make_shared<QCPLegend>();
//     // std::vector< std::shared_ptr<QCPPlottableLegendItem> > topLegendItems;
//     // std::vector< std::shared_ptr<QCPPlottableLegendItem> > bottomLegendItems;
//     topLegend = std::make_shared<QCPLegend>();
//     bottomLegend = std::make_shared<QCPLegend>();
//     topLegendItems.clear();
//     bottomLegendItems.clear();

//     // axis labels
//     topPlot->axis(QCPAxis::atBottom)->setLabel("Time [s]");
//     topPlot->axis(QCPAxis::atLeft)->setLabel("Amplitude");

//     bottomPlot->axis(QCPAxis::atBottom)->setLabel("Time [s]");
//     bottomPlot->axis(QCPAxis::atLeft)->setLabel("Amplitude");

//     // axis range
//     topPlot->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);
//     bottomPlot->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);

//     // Create graphs in the top subplot
//     QPen pen;
//     customPlot->addGraph(topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft));
//     pen.setColor(Qt::blue);
//     pen.setWidth(2);
//     pen.setStyle(Qt::SolidLine);
//     topPlot->graphs().back()->setPen(pen);
//     topPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
//     topPlot->graphs().back()->setName("Sine");

//     customPlot->addGraph(topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft));
//     pen.setColor(Qt::red);
//     pen.setWidth(4);
//     pen.setStyle(Qt::DashLine);
//     topPlot->graphs().back()->setPen(pen);
//     topPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
//     topPlot->graphs().back()->setName("Cosine");

//     // Create graphs in the bottom subplot
//     customPlot->addGraph(bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft));
//     pen.setColor(Qt::green);
//     pen.setWidth(2);
//     pen.setStyle(Qt::SolidLine);
//     bottomPlot->graphs().back()->setPen(pen);
//     bottomPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
//     bottomPlot->graphs().back()->setName("Tangent");

//     customPlot->addGraph(bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft));
//     pen.setColor(Qt::magenta);
//     pen.setWidth(4);
//     pen.setStyle(Qt::DashLine);
//     bottomPlot->graphs().back()->setPen(pen);
//     bottomPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
//     bottomPlot->graphs().back()->setName("Cotangent");

//     // legends
//     // auto topLegend = std::make_shared<QCPLegend>();
//     topPlot->insetLayout()->addElement(topLegend.get(), Qt::AlignTop | Qt::AlignRight);
//     topLegend->setVisible(true);
//     topLegend->setBrush(QBrush(QColor(255, 255, 255, 150)));
//     topLegend->setBorderPen(QPen(Qt::black));

//     // auto bottomLegend = std::make_shared<QCPLegend>();
//     bottomPlot->insetLayout()->addElement(bottomLegend.get(), Qt::AlignTop | Qt::AlignRight);
//     bottomLegend->setVisible(true);
//     bottomLegend->setBrush(QBrush(QColor(255, 255, 255, 150)));
//     bottomLegend->setBorderPen(QPen(Qt::black));

//     // Add graphs to the legends
//     topLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[0]) );
//     topLegend->addItem( topLegendItems.back().get() );
//     topLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[1]) );
//     topLegend->addItem( topLegendItems.back().get() );

//     bottomLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[0]) );
//     bottomLegend->addItem( bottomLegendItems.back().get() );
//     bottomLegendItems.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[1]) );
//     bottomLegend->addItem( bottomLegendItems.back().get() );

//     // Setup a timer to update the plot
//     QTimer *dataTimer = new QTimer(customPlot.get());
//     QObject::connect(dataTimer, &QTimer::timeout, [customPlot, topPlot, bottomPlot]() {
//         static double t = 0;
//         t += 0.1;
//         customPlot->graph(0)->addData(t, qSin(t));
//         customPlot->graph(0)->data()->removeBefore(t - 5);
//         customPlot->graph(1)->addData(t, qCos(t));
//         customPlot->graph(1)->data()->removeBefore(t - 5);
//         topPlot->axis(QCPAxis::atBottom)->setRange(t - 5, t);

//         customPlot->graph(2)->addData(t, qSin(2.0 * t));
//         customPlot->graph(2)->data()->removeBefore(t - 5);
//         customPlot->graph(3)->addData(t, qCos(2.0 * t));
//         customPlot->graph(3)->data()->removeBefore(t - 5);
//         bottomPlot->axis(QCPAxis::atBottom)->setRange(t - 5, t);

//         customPlot->replot();
//     });
//     dataTimer->start(100); // Update every 100 ms

//     { // debug
//         std::stringstream ss;
//         ss << "graph count = " << topPlot->graphs().size() << "\n";
//         ss << "graph count from plot = " << customPlot->graphCount() << "\n";
//         // ss << "legend item count = " << static_cast<int>( axis->LegendItems().size() ) << "\n";
//         // ss << "legend item count from legend = " << topPlot->_legend->itemCount() << "\n";
//         ss << "legend item count from plot = " << customPlot->legend->itemCount() << "\n";
//         // ss << "graph ID-Index map = \n" << axis->showGraphIdIndexMap();

//         std::cout << ss.str() << std::endl;

//     }
// }

// class MyWindow2 : public QMainWindow
// {
// public: 
//     std::shared_ptr<QWidget> centralWidget;
//     std::shared_ptr<QGridLayout> layout;
    
//     std::vector< std::shared_ptr<QCustomPlot> > plots;
//     std::vector< std::shared_ptr<QCPAxisRect> > axes;
//     std::vector< std::shared_ptr<QCPLegend> > legends;
//     std::vector< std::vector< std::shared_ptr<QCPPlottableLegendItem> > > legendItems;
// };

// void setupWindow2( std::shared_ptr<MyWindow2> window )
// {
//     // Create a central widget and a layout
//     // auto centralWidget = std::make_shared<QWidget>( window.get() );
//     // auto layout = std::make_shared<QGridLayout>( centralWidget.get() );
//     window->centralWidget = std::make_shared<QWidget>( window.get() );
//     window->layout = std::make_shared<QGridLayout>( window->centralWidget.get() );

//     setupPlot(window, window->customPlot1, window->topPlot1, window->bottomPlot1, window->topLegend1, window->bottomLegend1, window->topLegendItems1, window->bottomLegendItems1);
//     setupPlot(window, window->customPlot2, window->topPlot2, window->bottomPlot2, window->topLegend2, window->bottomLegend2, window->topLegendItems2, window->bottomLegendItems2);
//     { // debug
//         std::cout << "setup plots done" << std::endl;
//     }

//     window->layout->addWidget(window->customPlot1.get(),0,0);
//     window->layout->addWidget(window->customPlot2.get(),1,0);
//     { // debug
//         std::cout << "add plots to layout done" << std::endl;
//     }

//     window->centralWidget->setLayout(window->layout.get());
//     { // debug
//         std::cout << "set layout to central widget done" << std::endl;
//     }

//     window->setCentralWidget(window->centralWidget.get());
//     { // debug
//         std::cout << "set central widget to window done" << std::endl;
//     }

//     window->resize(800, 600);
//     { // debug
//         std::cout << "resize window done" << std::endl;
//     }

//     return;

// }

void multiple_windows(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::shared_ptr<MyWindow> window1 = std::make_shared<MyWindow>();
    setupWindow(window1, 0, 0, 800, 600);

    std::shared_ptr<MyWindow> window2 = std::make_shared<MyWindow>();
    setupWindow(window2, 900, 200, 800, 600);

    window1->show();
    window2->show();

    a.exec();
}

void single_window(int argc, char *argv[], const int px, const int py)
{
    QApplication a(argc, argv);
    std::shared_ptr<QMainWindow> window = std::make_shared<QMainWindow>();
    
    std::shared_ptr<QCustomPlot> customPlot = std::make_shared<QCustomPlot>();
    std::shared_ptr<QCPAxisRect> topPlot = std::make_shared<QCPAxisRect>(customPlot.get());
    std::shared_ptr<QCPAxisRect> bottomPlot = std::make_shared<QCPAxisRect>(customPlot.get());
    std::shared_ptr<QCPLegend> topLegend = std::make_shared<QCPLegend>();
    std::shared_ptr<QCPLegend> bottomLegend = std::make_shared<QCPLegend>();
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > legendItems;
    

    std::shared_ptr<QCustomPlot> customPlot1 = std::make_shared<QCustomPlot>();
    std::shared_ptr< QCPAxisRect> topPlot1 = std::make_shared<QCPAxisRect>(customPlot1.get());
    std::shared_ptr< QCPAxisRect> bottomPlot1 = std::make_shared<QCPAxisRect>(customPlot1.get());
    std::shared_ptr<QCPLegend> topLegend1 = std::make_shared<QCPLegend>();
    std::shared_ptr<QCPLegend> bottomLegend1 = std::make_shared<QCPLegend>();
    std::vector< std::shared_ptr<QCPPlottableLegendItem> > legendItems1;
    // QSharedPointer<QCustomPlot> customPlot; 

    // Setup customPlot as central widget of window
    std::shared_ptr<QWidget> centralWidget = std::make_shared<QWidget>( window.get() );
    std::shared_ptr<QGridLayout> layout = std::make_shared<QGridLayout>( centralWidget.get() );
    layout->addWidget( customPlot.get(),0,0 );
    layout->addWidget( customPlot1.get(),1,0 );
    centralWidget->setLayout( layout.get() );
    window->setCentralWidget( centralWidget.get() );

    // window->setCentralWidget(customPlot.get());

    {
    // Create axes
    customPlot->plotLayout()->clear(); // Clear the default axis rect
    customPlot->plotLayout()->addElement(0, 0, topPlot.get());
    customPlot->plotLayout()->addElement(1, 0, bottomPlot.get());

    topPlot->axis(QCPAxis::atBottom)->setLabel("Time [s]");
    topPlot->axis(QCPAxis::atLeft)->setLabel("Amplitude");
    topPlot->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);

    bottomPlot->axis(QCPAxis::atBottom)->setLabel("Time [s]");
    bottomPlot->axis(QCPAxis::atLeft)->setLabel("Amplitude");
    bottomPlot->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);

    // Create graphs
    // top graph
    QPen pen;
    customPlot->addGraph( topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::blue);
    pen.setWidth(2); 
    pen.setStyle(Qt::SolidLine); 
    topPlot->graphs().back()->setPen(pen);
    topPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    topPlot->graphs().back()->setName("Sine-1");
    
    customPlot->addGraph( topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::red);
    pen.setWidth(4); 
    pen.setStyle(Qt::DashLine);
    topPlot->graphs().back()->setPen(pen);
    topPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    topPlot->graphs().back()->setName("Cosine-1");
    
    // bottom graph
    customPlot->addGraph( bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    bottomPlot->graphs().back()->setPen(pen);
    bottomPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    bottomPlot->graphs().back()->setName("Sine-2");

    customPlot->addGraph( bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::red);
    pen.setWidth(4);
    pen.setStyle(Qt::DashLine);
    bottomPlot->graphs().back()->setPen(pen);
    bottomPlot->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    bottomPlot->graphs().back()->setName("Cosine-2");

    // legends
    // auto topLegend = std::make_shared<QCPLegend>();
    topPlot->insetLayout()->addElement(topLegend.get(), Qt::AlignTop | Qt::AlignRight);
    topLegend->setVisible(true);
    topLegend->setBrush(QBrush(QColor(255, 255, 255, 150)));
    topLegend->setBorderPen(QPen(Qt::black));

    // auto bottomLegend = std::make_shared<QCPLegend>();
    bottomPlot->insetLayout()->addElement(bottomLegend.get(), Qt::AlignTop | Qt::AlignRight);
    bottomLegend->setVisible(true);
    bottomLegend->setBrush(QBrush(QColor(255, 255, 255, 150)));
    bottomLegend->setBorderPen(QPen(Qt::black));

    // Add graphs to the legends    
    // auto legendItem0 = std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[0]);
    // topLegend->addItem(legendItem0.get());
    // auto legendItem1 = std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[1]);
    // topLegend->addItem(legendItem1.get());

    // auto legendItem2 = std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[0]);
    // bottomLegend->addItem(legendItem2.get());
    // auto legendItem3 = std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[1]);
    // bottomLegend->addItem(legendItem3.get());

    legendItems.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[0]) );
    topLegend->addItem( legendItems.back().get() );
    legendItems.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend.get(), topPlot->graphs()[1]) );
    topLegend->addItem( legendItems.back().get() );

    legendItems.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[0]) );
    bottomLegend->addItem( legendItems.back().get() );
    legendItems.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend.get(), bottomPlot->graphs()[1]) );
    bottomLegend->addItem( legendItems.back().get() );

    }

    {
    // Create subplots
    customPlot1->plotLayout()->clear(); // Clear the default axis rect
    customPlot1->plotLayout()->addElement(0, 0, topPlot1.get());
    customPlot1->plotLayout()->addElement(1, 0, bottomPlot1.get());

    topPlot1->axis(QCPAxis::atBottom)->setLabel("Time [s]");
    topPlot1->axis(QCPAxis::atLeft)->setLabel("Amplitude");
    topPlot1->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);

    bottomPlot1->axis(QCPAxis::atBottom)->setLabel("Time [s]");
    bottomPlot1->axis(QCPAxis::atLeft)->setLabel("Amplitude");
    bottomPlot1->axis(QCPAxis::atLeft)->setRange(-1.5, 1.5);

    // Create graphs
    // top graph
    QPen pen;
    customPlot1->addGraph( topPlot1->axis(QCPAxis::atBottom), topPlot1->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    topPlot1->graphs().back()->setPen(pen);
    topPlot1->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    topPlot1->graphs().back()->setName("Sine-4");

    customPlot1->addGraph( topPlot1->axis(QCPAxis::atBottom), topPlot1->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::red);
    pen.setWidth(4);
    pen.setStyle(Qt::DashLine);
    topPlot1->graphs().back()->setPen(pen);
    topPlot1->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    topPlot1->graphs().back()->setName("Cosine-4");

    // bottom graph
    customPlot1->addGraph( bottomPlot1->axis(QCPAxis::atBottom), bottomPlot1->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::blue); 
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    bottomPlot1->graphs().back()->setPen(pen);
    bottomPlot1->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    bottomPlot1->graphs().back()->setName("Sine-8");

    customPlot1->addGraph( bottomPlot1->axis(QCPAxis::atBottom), bottomPlot1->axis(QCPAxis::atLeft) );
    pen.setColor(Qt::red);
    pen.setWidth(4);
    pen.setStyle(Qt::DashLine);
    bottomPlot1->graphs().back()->setPen(pen);
    bottomPlot1->graphs().back()->setLineStyle(QCPGraph::LineStyle::lsLine);
    bottomPlot1->graphs().back()->setName("Cosine-8");

    // legends
    // auto topLegend = std::make_shared<QCPLegend>();
    topPlot1->insetLayout()->addElement(topLegend1.get(), Qt::AlignTop | Qt::AlignRight);
    topLegend1->setVisible(true);
    topLegend1->setBrush(QBrush(QColor(255, 255, 255, 150)));
    topLegend1->setBorderPen(QPen(Qt::black));

    // auto bottomLegend = std::make_shared<QCPLegend>();
    bottomPlot1->insetLayout()->addElement(bottomLegend1.get(), Qt::AlignTop | Qt::AlignRight);
    bottomLegend1->setVisible(true);
    bottomLegend1->setBrush(QBrush(QColor(255, 255, 255, 150)));
    bottomLegend1->setBorderPen(QPen(Qt::black));

    // Add graphs to the legends
    legendItems1.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend1.get(), topPlot1->graphs()[0]) );
    topLegend1->addItem( legendItems1.back().get() );
    legendItems1.push_back( std::make_shared<QCPPlottableLegendItem>(topLegend1.get(), topPlot1->graphs()[1]) );
    topLegend1->addItem( legendItems1.back().get() );

    legendItems1.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend1.get(), bottomPlot1->graphs()[0]) );
    bottomLegend1->addItem( legendItems1.back().get() );
    legendItems1.push_back( std::make_shared<QCPPlottableLegendItem>(bottomLegend1.get(), bottomPlot1->graphs()[1]) );
    bottomLegend1->addItem( legendItems1.back().get() );

    }

    // Setup a timer to update the plot
    QTimer dataTimer;
    QObject::connect(&dataTimer, &QTimer::timeout, [&customPlot, &topPlot, &bottomPlot]() {
        static double t = 0;
        t += 0.1;

        topPlot->graphs()[0]->addData(t, qSin(t));
        topPlot->graphs()[0]->data()->removeBefore(t-5);
        topPlot->graphs()[1]->addData(t, qCos(t));
        topPlot->graphs()[1]->data()->removeBefore(t-5);
        topPlot->axis(QCPAxis::atBottom)->setRange(t-5,t);

        bottomPlot->graphs()[0]->addData(t, qSin(2.0*t));
        bottomPlot->graphs()[0]->data()->removeBefore(t-5);
        bottomPlot->graphs()[1]->addData(t, qCos(2.0*t));
        bottomPlot->graphs()[1]->data()->removeBefore(t-5);
        bottomPlot->axis(QCPAxis::atBottom)->setRange(t-5,t);

        customPlot->replot();
    });
    QObject::connect(&dataTimer, &QTimer::timeout, [&customPlot1, &topPlot1, &bottomPlot1]() {
        static double t = 0;
        t += 0.1;

        topPlot1->graphs()[0]->addData(t, qSin(4.0*t));
        topPlot1->graphs()[0]->data()->removeBefore(t-5);
        topPlot1->graphs()[1]->addData(t, qCos(4.0*t));
        topPlot1->graphs()[1]->data()->removeBefore(t-5);
        topPlot1->axis(QCPAxis::atBottom)->setRange(t-5,t);

        bottomPlot1->graphs()[0]->addData(t, qSin(8.0*t));
        bottomPlot1->graphs()[0]->data()->removeBefore(t-5);
        bottomPlot1->graphs()[1]->addData(t, qCos(8.0*t));
        bottomPlot1->graphs()[1]->data()->removeBefore(t-5);
        bottomPlot1->axis(QCPAxis::atBottom)->setRange(t-5,t);

        customPlot1->replot();
    });
    dataTimer.start(20); // Update every 100 ms
    

    // window->resize(800, 600);
    window->setGeometry(px, py, 800, 600);
    window->show();

    a.exec();

    return; 
}

int main(int argc, char *argv[])
{
    // single_window(argc, argv);

    multiple_windows(argc, argv);
    
    // QApplication a(argc, argv);

    // QMainWindow window1; 
    // setupWindow(&window1);

    // QMainWindow window2;
    // setupWindow(&window2);

    // return a.exec();
}