/*
 *  mafNodeGraphicWidget.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 11/02/13.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNODEGRAPHICWIDGET_H
#define MAFNODEGRAPHICWIDGET_H

#include "mafGUIDefinitions.h"
#include "mafDiagramScene.h"
#include <mafNodeConnectorGraphicWidget.h>

#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsProxyWidget>
#include <QtGui>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

namespace mafGUI {

// forwar classes
class mafNodeConnectionGraphicWidget;

/**
Class name: mafNodeGraphicWidget
Class which represent the basic class for node widget.
*/
class mafNodeGraphicWidget : public QGraphicsProxyWidget {
	Q_OBJECT
public:
    /// type enum
    enum { Type = UserType + 666 + 1 };

    /// retrieve type.
    int type() const;

    /// object constructor.
    mafNodeGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, Qt::WindowFlags wFlags = 0);

    /// object destructor.
    virtual ~mafNodeGraphicWidget();

    /// remove all connections.
    void deleteConnections();

    /// add connector to the node.
    void addConnector(mafNodeConnectorGraphicWidget* nc);

    /// paint method.
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /// set a widget to the node.
    void setWidget(QWidget *widget);

    /// retrieve the bounding rect.
    QRectF boundingRect() const;

    /// retrieve the shape of the node.
    QPainterPath shape() const;

    QList<mafNodeConnectorGraphicWidget *> &connectors();

public slots:
        void deleted();
        void deleted(int result);

        //dw try overriding QWidget slot
        void hide();

/*
void 	accepted ()
void 	finished ( int result )
void 	rejected ()
*/

//dw666 orig:	QList<mafNodeConnectorGraphicWidget *> connectors;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

	bool shouldMoveNode(QGraphicsSceneMouseEvent *mouseEvent);
	//const static QStringList shouldNotMoveTypes << "foo" << "bar";
	//const static char* shouldNotMoveTypes = {};
	const static char* shouldMoveOnClickTypes[];


	void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );


	bool mControlResizeHandles;
	bool mNoResize;

	//dw 669: new
	void resizeEvent ( QGraphicsSceneResizeEvent * event );

    QList<mafNodeConnectorGraphicWidget *> m_Connectors;

private:
    QMenu *mContextMenu;



	void updateConnectorsPos();

	bool isMoving;

	void removeWigetFromConnectors();

	void debugPaint(QPainter *painter);

	int mMaxRadius;


};

} // end namespace mafGUI

#endif // MAFNODEGRAPHICWIDGET
