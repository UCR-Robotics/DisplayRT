# Usage Manual

This document provides information regarding display properties like 
pen colors, pen styles, and line style.

## Available Pen Colors
- **black**
- **white**
- **gray**
- **red**
- **green**
- **blue**
- **cyan**
- **magenta**
- **yellow**

## Available Pen Styles
- **solid**
- **dash**
- **dot**
- **dashdot**
- **dashdotdot**

## Available Line Styles
- **none**
- **line**
- **step_center**
- **step_left**
- **step_right**
- **impulse**

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