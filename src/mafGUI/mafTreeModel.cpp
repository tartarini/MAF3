/*
 *  mafTreeModel.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeModel.h"

using namespace mafCore;
using namespace mafGUI;
using namespace mafEventBus;

mafTreeModel::mafTreeModel(QObject *parent)
    : QStandardItemModel(parent) , m_Hierarchy(NULL), m_CurrentItem(NULL) {
}

void mafTreeModel::initialize() {
    REQUIRE(m_Hierarchy != NULL);
    //header to write
    //setHorizontalHeaderLabels(QStringList() << tr("Item/Item/..."));
    for (int column = 1; column < columnCount(); ++column) {
        horizontalHeaderItem(column)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
    }

    buildModel();
}

void mafTreeModel::buildModel(bool init) {
    //create root
    if(init == true) {
        m_Hierarchy->moveTreeIteratorToRootNode();
        m_CurrentItem = new mafTreeItem(m_Hierarchy->currentData() , false);
        setItem(0, 0, m_CurrentItem);
        m_ItemsList.push_back(m_CurrentItem);
    }

    int i = 0, size = m_Hierarchy->currentNumberOfChildren();
    for(;i < size; ++i) {
      m_Hierarchy->moveTreeIteratorToNthChild(i);
      QObject *obj = m_Hierarchy->currentData();

      mafTreeItem *item = this->createNewItem(m_CurrentItem, obj, false);
      m_CurrentItem = item;
      buildModel(false);
      m_Hierarchy->moveTreeIteratorToParent();
      m_CurrentItem = (mafTreeItem *)m_CurrentItem->parent();       
    }
}

bool mafTreeModel::submit() {
    if (m_CurrentItem->parent() == NULL) {
        return false;
    }
    m_CurrentItem->data()->setObjectName(m_CurrentItem->text());
    return true;
}

void mafTreeModel::setHierarchy(mafHierarchy *hierarchy) {
    if(m_Hierarchy) {
         disconnect(m_Hierarchy, SIGNAL(itemAttached(QObject*,QObject*)), this, SLOT(itemAttached(QObject*,QObject*)));
         disconnect(m_Hierarchy, SIGNAL(itemDetached(QObject*)), this, SLOT(itemDetached(QObject*)));
         disconnect(m_Hierarchy, SIGNAL(itemReparent(QObject*,QObject*)), this, SLOT(itemReparent(QObject*,QObject*)));
    }

    m_Hierarchy = hierarchy;
    connect(m_Hierarchy, SIGNAL(itemAttached(QObject*,QObject*)), this, SLOT(itemAttached(QObject*,QObject*)));
    connect(m_Hierarchy, SIGNAL(itemDetached(QObject*)), this, SLOT(itemDetached(QObject*)));
    connect(m_Hierarchy, SIGNAL(itemReparent(QObject*,QObject*)), this, SLOT(itemReparent(QObject*,QObject*)));
    connect(m_Hierarchy, SIGNAL(destroyed()), this, SLOT(hierarchyDestroyed()));

    initialize();
}

void mafTreeModel::clear() {
    QStandardItemModel::clear();
    m_ItemsList.clear();
    //initialize();
}

void mafTreeModel::itemAttached(QObject *item, QObject *parent) {
    QModelIndex index = this->indexFromData(parent);
    this->insertNewItem(AsChild, item, index);
    emit itemAdded(index);
}

void mafTreeModel::itemDetached(QObject *item) {
    mafTreeItem *temp = (mafTreeItem *)this->itemFromIndex(this->indexFromData(item));
    bool removed = this->removeRows(temp->index().row(), 1, this->indexFromData(item).parent());
}

mafTreeItem *mafTreeModel::createNewItem(mafTreeItem *parent, QObject *obj, bool done) {
    mafTreeItem *item = new mafTreeItem(obj,done);
    parent->appendRow(item);
    //Update check state
    bool visibility = obj->property("visibility").toBool();
    if(item->isCheckable()){
      item->setCheckState(visibility ? Qt::Checked : Qt::Unchecked);
      if(visibility) {
        this->setProperty("visibility", visibility);
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase*, (mafObjectBase*)obj));
        argList.append(mafEventArgument(bool, visibility));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.sceneNodeShow", mafEventTypeLocal, &argList);
      }
    }

    m_ItemsList.push_back(item);
    return item;
}

void mafTreeModel::hierarchyDestroyed() {
  m_Hierarchy = NULL;
}


void mafTreeModel::itemReparent(QObject *item, QObject *parent) {
    QModelIndex index = this->indexFromData(parent);
    this->insertNewItem(AsChild, item, index);
}

void mafTreeModel::selectItem(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);
    m_CurrentItem = (mafTreeItem *)this->itemFromIndex(selected.indexes().at(0));
}


void mafTreeModel::insertNewItem(Insert insert,
             QObject *obj, const QModelIndex &index) {
    mafTreeItem *parent;
    if (insert == AtTopLevel) {
        parent = (mafTreeItem *)invisibleRootItem();
    } else {
        if (index.isValid()) {
            parent = (mafTreeItem *) itemFromIndex(index);
            if (!parent)
                return;
            if (insert == AsSibling)
                parent = (mafTreeItem *)(parent->parent() ? parent->parent()
                                          : invisibleRootItem());
        } else {
            return;
        }
    }

    mafTreeItem *oldItem = (mafTreeItem *)this->itemFromIndex(indexFromData(obj));
    mafTreeItem *newItem = createNewItem(parent, obj, false);
    if (oldItem) {
        int i = 0, size = oldItem->rowCount();
        for(; i < size; ++i) {
            QObject *child = ((mafTreeItem *)oldItem->child(i))->data();
            insertNewItem(AsChild, child, newItem->index());
        }
    }
}

bool mafTreeModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (parent.isValid()) {
        mafTreeItem *temp = (mafTreeItem *)this->itemFromIndex(parent.child(row,0));
        removeFromList(temp->index());
        return QStandardItemModel::removeRows(row, count, parent);
    }
}

void mafTreeModel::removeFromList(const QModelIndex &index) {
    mafTreeItem *temp = (mafTreeItem *)this->itemFromIndex(index);
    if(temp->parent() == NULL) {
        qDebug() << mafTr("Impossible removing the root");
        return;
    }
    int i = 0, size = temp->rowCount();
    for(; i < size; ++i) {
        removeFromList(temp->index().child(i, 0));
    }

    m_ItemsList.removeOne(temp);
}

QModelIndex mafTreeModel::currentIndex() {
    if(m_CurrentItem) {
        return m_CurrentItem->index();
    }

    return QModelIndex();
}

QModelIndex mafTreeModel::indexFromData(QObject *data) {
    bool found = false;
    QListIterator<mafTreeItem *> it(m_ItemsList);
    while (it.hasNext()) {
        mafTreeItem *ci = it.next();
         QObject *check = ci->data();
         if(check == data) {
             found = true;
             return ci->index();
         }
     }
    if (!found) {
        qDebug() << mafTr("Element not found");
        return QModelIndex();
    }
}


