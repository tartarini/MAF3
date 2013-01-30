#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsLineItem>
#include "mafNodeGraphicWidget.h"
#include "mafNodeConnectorGraphicWidget.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class mafNodeConnectorGraphicWidget;

//! [0]
class mafNodeConnectionGraphicWidget : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 666 + 0 };
	int type() const
        { return Type;}

        virtual ~mafNodeConnectionGraphicWidget();
    mafNodeConnectionGraphicWidget(mafNodeConnectorGraphicWidget *startConnector, mafNodeConnectorGraphicWidget *endConnector,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, bool bidirectional = true);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { mColor = color; }
    mafNodeConnectorGraphicWidget *startConnector() const
        { return mStartConnector; }
    mafNodeConnectorGraphicWidget *endConnector() const
        { return mEndConnector; }

	void updatePosition();

	bool bidirectional();
	bool setBidirectional(bool bidirectional);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

	//returns used/created control points (eg for debug draw,...)
	void recreatePath(QPointF& controlPoint1, QPointF& controlPoint2);
        QPolygonF createArrowPoly(QPainterPath& p, mafNodeConnectorGraphicWidget* conn);

private:
    mafNodeConnectorGraphicWidget *mStartConnector;
    mafNodeConnectorGraphicWidget *mEndConnector;
    QColor mColor;
    
	qreal arrowSize;
	bool mBidirectional;

	void debugPaint(QPainter *painter, QPointF& controlPoint1, QPointF& controlPoint2);


};
//! [0]

#endif