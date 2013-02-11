//dw
#ifndef mafNodeGraphicWidget_H
#define mafNodeGraphicWidget_H

#include "mafGUIDefinitions.h"
#include "mafDiagramScene.h"


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
/*//dw
class QGraphicsProxyWidget;*/
QT_END_NAMESPACE

class mafNodeConnectorGraphicWidget;
class mafNodeConnectionGraphicWidget;

//! [0]
class mafNodeGraphicWidget : public QGraphicsProxyWidget {
	Q_OBJECT
public:
    enum { Type = UserType + 666 + 1 };
	int type() const
        { return Type;}

    mafNodeGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, Qt::WindowFlags wFlags = 0);
        virtual ~mafNodeGraphicWidget();
    void deleteConnections();

    void addConnector(mafNodeConnectorGraphicWidget* nc);


	//dw
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setWidget(QWidget *widget);

    QRectF boundingRect() const;
    QPainterPath shape() const;

    //void update(const QRectF & rect = QRectF());
    QList<mafNodeConnectorGraphicWidget *> connectors;

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


private:
    QMenu *mContextMenu;



	void updateConnectorsPos();

	bool isMoving;

	void removeWigetFromConnectors();

	void debugPaint(QPainter *painter);

	int mMaxRadius;


};


//! [0]


#endif
