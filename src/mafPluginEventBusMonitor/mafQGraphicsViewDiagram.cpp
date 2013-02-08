/*
 *  mafViewEventBusMonitor.cpp
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafQGraphicsViewDiagram.h"

#include <QPaintEvent>
#include <QWidget>

#include <QPaintEvent>

using namespace mafPluginEventBusMonitor;

mafQGraphicsViewDiagram::mafQGraphicsViewDiagram(QGraphicsScene* scene): QGraphicsView(scene) {}
void mafQGraphicsViewDiagram::paintEvent ( QPaintEvent * event) {
        QRect r = event->region().boundingRect();
        QRect r1 = scene()->itemsBoundingRect().toRect();

        QPaintEvent *newEvent=new QPaintEvent(r1);
        QGraphicsView::paintEvent(newEvent);
        delete newEvent;
}
