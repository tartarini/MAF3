/*
 *  mafDiagramScene.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 11/02/13.
 *  Copyright 2013 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDIAGRAMSCENE_H
#define MAFDIAGRAMSCENE_H

#include <QGraphicsScene>

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

namespace mafGUI {

// forward classes
class mafNodeGraphicWidget;

/*
Class Name: mafDiagramScene
Manager of the scene in which adding items.
*/
class MAFGUISHARED_EXPORT mafDiagramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /// Modality in which managing nodes or generaic items
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem, InsertNode, InsertNode2 };

    /// object constructor.
    mafDiagramScene(QMenu *itemMenu, QObject *parent = 0);

    /// retrieve line color.
    QColor lineColor() const;

    /// set line color.
    void setLineColor(const QColor &color);

    /// put node in the scene.
    void insertNode(mafNodeGraphicWidget *node);

    /// put an arrow in the scene.
    void insertArrow(mafNodeConnectionGraphicWidget *node);

public slots:
    /// set the modality of the scene.
    void setMode(Mode mode);

    /// activate/deactivate debug mode.
    void setDebugDraw(bool enabled = true);

    /// check if in debug mode.
    bool isDebugDraw() { return mDebugDraw; }

    /// retrieve the menu.
    QMenu *menu();

    /// retrieve the menu.
    void setMenu(QMenu *menu);

signals:
    /// signal emitted when the node is inserted.
    void nodeInserted(QGraphicsItem *item);

    /// signal emitted when the specific node is inserted.
    void mafNodeGraphicWidgetInserted(mafNodeGraphicWidget *item);

    /// signal emitted when the object is selected.
    void itemSelected(QGraphicsItem *item);

protected:
    /// mouse press event handled.
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

    /// mouse move event handled.
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

    /// mouse release event handled.
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    QMenu *mItemMenu;

private:
    /// check if the item has been changed.
    bool isItemChange(int type);


    bool leftButtonDown;
    QPointF startPoint;

    mafNodeConnectionGraphicWidget* tmpArrow;
    mafNodeConnectorGraphicWidget* tmpConnector;
    mafNodeConnectorGraphicWidget* existingConnector;
    mafNodeConnectorGraphicWidget* lastHighlighted;
    QColor mLineColor;
    Mode mMode;

    bool mDebugDraw;
};

} // namespace mafGUI

#endif // MAFDIAGRAMSCENE_H
