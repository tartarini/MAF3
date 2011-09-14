/*
 *  mafTreeWidget.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeWidget.h"
#include "mafTreeModel.h"
#include <QMenu>

using namespace mafGUI;

mafTreeWidget::mafTreeWidget(QWidget *parent) : QTreeView(parent) {
    m_ActionShow = new QAction(mafTr("Show"), this);
    m_ActionDelete = new QAction(mafTr("Delete"), this);
}

void mafTreeWidget::contextMenuEvent(QContextMenuEvent *e) {
    QList<QAction *> actions;
    if (this->indexAt(e->pos()).isValid()) {
        actions.append(m_ActionShow);
        actions.append(m_ActionDelete);
    }
    if (actions.count() > 0) {
        QMenu::exec(actions, e->globalPos());
    }
}

void mafTreeWidget::dragMoveEvent(QDragMoveEvent *event) {
    QTreeView::dragMoveEvent(event);
    QPoint p = event->pos();
    QModelIndex item = indexAt(p);
    
    if(item.isValid()) {
        if(!this->isExpanded(item)) {
            this->expand(item);
        }
    }
}

bool mafTreeWidget::event(QEvent *event) {
    bool result = QTreeView::event(event);
    
    if(event->type() == QEvent::ChildRemoved) {
        this->clearSelection();
        mafTreeModel *m = (mafTreeModel *)model();
        this->selectionModel()->setCurrentIndex(m->currentIndex(), QItemSelectionModel::Select);
    } 
    
    return result;
}