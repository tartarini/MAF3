/*
 *  mafNodeConnectionGraphicWidget.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 11/02/13.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNODECONNECTIONGRAPHICWIDGET_H
#define MAFNODECONNECTIONGRAPHICWIDGET_H

#include "mafGUIDefinitions.h"
#include "mafNodeGraphicWidget.h"
#include "mafNodeConnectorGraphicWidget.h"

#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

namespace mafGUI {

/**
Class Name:mafNodeConnectionGraphicWidget.
Class that represents the widget for connect two node widgets.
*/
class MAFGUISHARED_EXPORT mafNodeConnectionGraphicWidget : public QGraphicsPathItem {
public:

    /// type enum
    enum { Type = UserType + 1001 };

    /// retrieve type.
    int type() const;

    /// object destructor
    virtual ~mafNodeConnectionGraphicWidget();

    /// object constructor
    mafNodeConnectionGraphicWidget(mafNodeConnectorGraphicWidget *startConnector, mafNodeConnectorGraphicWidget *endConnector, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, bool bidirectional = true);

    /// retrieve bounding rect area.
    QRectF boundingRect() const;

    /// retrieve the shape of the graphic item.
    QPainterPath shape() const;

    /// set the color of the connector
    void setColor(const QColor &color)
        { mColor = color; }

    /// retrieve the start connector
    mafNodeConnectorGraphicWidget *startConnector() const
        { return mStartConnector; }

    /// retrieve the end connector
    mafNodeConnectorGraphicWidget *endConnector() const
        { return mEndConnector; }

    /// refresh the position
	void updatePosition();

    /// retrieve flag for check if the connector is bidimentional
	bool bidirectional();

    /// set the bidirectional flag
	bool setBidirectional(bool bidirectional);

protected:
    /// paint call back
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

    /// returns used/created control points (eg for debug draw,...)
	void recreatePath(QPointF& controlPoint1, QPointF& controlPoint2);
        QPolygonF createArrowPoly(QPainterPath& p, mafNodeConnectorGraphicWidget* conn);

private:
    mafNodeConnectorGraphicWidget *mStartConnector;
    mafNodeConnectorGraphicWidget *mEndConnector;
    QColor mColor;
    
    qreal arrowSize;
    bool mBidirectional;

    /// function for debugging Paint
    void debugPaint(QPainter *painter, QPointF& controlPoint1, QPointF& controlPoint2);

};

} // end namespace mafGUI

#endif
