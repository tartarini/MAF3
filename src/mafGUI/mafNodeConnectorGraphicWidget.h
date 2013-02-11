/*
 *  mafNodeConnectorGraphicWidget.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 11/02/13.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNODECONNECTORGRAPHICWIDGET_H
#define MAFNODECONNECTORGRAPHICWIDGET_H

#include <QGraphicsItem>
#include <QList>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

namespace mafGUI {

// forward classes
class mafNodeGraphicWidget;
class mafNodeConnectionGraphicWidget;

/**
Class Name: mafNodeConnectorGraphicWidget
Representation of the connector widget in the node widget.
*/
class mafNodeConnectorGraphicWidget : public QGraphicsItem {
public:        
        enum { Type = UserType + 666 + 6 };
        int type() const
           { return Type;}

        /// enum connector type
        enum ConnectorType { In=1, Out=2, InOut=3 };

        /// enum connector alignment
        enum ConnectorAlignment { None=0, Left=1, Right=2, Bottom=3, Top=4 };

        /// retrieve connector type
        ConnectorType connectorType() const;

        /// set connector type
        void setConnectorType(ConnectorType c);

        ConnectorAlignment connectorAlignment() const;

        void setConnectorAlignment(ConnectorAlignment alignment);

	bool singleConnection() const { return mSingleConnection; }
	void setSingleConnection(bool singleConnection) {
		mSingleConnection = singleConnection;
	}

	bool selfConnections() const { return mSelfConnections; }
	void setSelfConnections(bool selfConnections) {
		mSelfConnections = selfConnections;
	}


    mafNodeConnectorGraphicWidget(mafNodeGraphicWidget *parent, QGraphicsScene *scene, /*QLabel*/QWidget* widget, ConnectorType  con = In, const ConnectorAlignment connectorAlignment = Left, const bool singleConnection = false, const bool disableWidgetOnConnection = false, const int radius = 7, const bool selfConnections = false);
    virtual ~mafNodeConnectorGraphicWidget();
/*
    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();
*/
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	//dw
	void updatePosition();
	//void updatePositionGeometry();
        void addConnection(mafNodeConnectionGraphicWidget *arrow);
        void deleteConnection(mafNodeConnectionGraphicWidget *arrow);
	void deleteConnections();
        void removeConnection(mafNodeConnectionGraphicWidget *arrow);

	void setHighlight(bool highlight);

//private:
	int mRadius;
	ConnectorType mConnectorType;
        mafNodeGraphicWidget *parent;
	QWidget* mWidget;
	//QWidget* label;
	//bool right;
	ConnectorAlignment mConnectorAlignment;
        QList<mafNodeConnectionGraphicWidget *> arrows;
	QColor darkColor;
	bool highlight;
	bool mSingleConnection;
	bool mSelfConnections;
	bool mDisableWidgetOnConnection;

	//ugly, but node ctor is called after wiget is removed, but delete of connector would otherwise activate label widget
    void removeWidget();

	void update(const QRectF & rect = QRectF());

protected:
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	//void mouseOverEvent(QGraphicsSceneMouseEvent *event);
	//void enterEvent ( QEvent * event );

private:

	void debugPaint(QPainter *painter);
};

} // end namespace mafGUI

#endif // MAFNODECONNECTORGRAPHICWIDGET
