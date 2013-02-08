/*
 *  mafViewScriptInterpreter.h
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFQGRAPHICSVIEW_H
#define MAFQGRAPHICSVIEW_H
 
#include "mafPluginEventBusMonitor_global.h"
#include "mafPluginEventBusMonitorDefinitions.h"

#include <QGraphicsView>

//class forwarding
class QPaintEvent;

namespace mafPluginEventBusMonitor {

/**
 Class name: mafQGraphicsViewDiagram
 Simple class for drawing the correct area
*/

class MAFPLUGINEVENTBUSMONITORSHARED_EXPORT mafQGraphicsViewDiagram : public QGraphicsView {
Q_OBJECT
public:
    // object constructor.
    mafQGraphicsViewDiagram(QGraphicsScene* scene);
protected:
    // paint event method.
    void paintEvent ( QPaintEvent * event);
};

}

#endif // MAFQGRAPHICSVIEW_H
