#ifndef mafDiagramScene_H
#define mafDiagramScene_H

#include <QGraphicsScene>

//dw
#include "mafNodeGraphicWidget.h"
#include "mafNodeConnectionGraphicWidget.h"
#include "mafNodeConnectorGraphicWidget.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//dw new
class mafNodeGraphicWidget;

//! [0]
class mafDiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem, InsertNode, InsertNode2 };

    mafDiagramScene(QMenu *itemMenu, QObject *parent = 0);
    QColor lineColor() const
        { return mLineColor; }
    void setLineColor(const QColor &color);

    QMenu *mItemMenu;

    //extended API for draw by functions.
    void drawNode(mafNodeGraphicWidget *node);
    void drawArrow();
    void deleteNode();
    void deleteArrow();


public slots:
    void setMode(Mode mode);
    //void setItemType(DiagramItem::DiagramType type);
    //void editorLostFocus(DiagramTextItem *item);

	void setDebugDraw(bool enabled = true);
	bool isDebugDraw() { return mDebugDraw; }

signals:
    //void itemInserted(DiagramItem *item);
    //void textInserted(QGraphicsTextItem *item);
	//dw
	void nodeInserted(QGraphicsItem *item);
        void mafNodeGraphicWidgetInserted(mafNodeGraphicWidget *item);

    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

	QColor mLineColor;
	Mode mMode;
private:
    bool isItemChange(int type);

    //DiagramItem::DiagramType mItemType;
    
    bool leftButtonDown;
    QPointF startPoint;

        mafNodeConnectionGraphicWidget* tmpArrow;
        mafNodeConnectorGraphicWidget* tmpConnector;
        mafNodeConnectorGraphicWidget* existingConnector;
        mafNodeConnectorGraphicWidget* lastHighlighted;

	bool mDebugDraw;
};
//! [0]

#endif
