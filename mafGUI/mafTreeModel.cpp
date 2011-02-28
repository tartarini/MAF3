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

mafTreeModel::mafTreeModel(QObject *parent)
    : QStandardItemModel(parent) , m_Hierarchy(NULL), m_CurrentItem(NULL) {
}

void mafTreeModel::initialize() {
    REQUIRE(m_Hierarchy != NULL);
    //header to write
    setHorizontalHeaderLabels(QStringList() << tr("Item/Item/..."));
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

    int index = 0, size = m_Hierarchy->currentNumberOfChildren();
    for(;index < size; ++index) {
        m_Hierarchy->moveTreeIteratorToNthChild(index);
        QObject *obj = m_Hierarchy->currentData();
//        QString name = obj->objectName();
        mafTreeItem *item = new mafTreeItem(obj , false);
        m_CurrentItem->insertRow(index, item);
        setItem(index, 0, m_CurrentItem);
        m_CurrentItem = item;
        m_ItemsList.push_back(m_CurrentItem);
        buildModel(false);
        m_Hierarchy->moveTreeIteratorToParent();
        m_CurrentItem = (mafTreeItem *)m_CurrentItem->parent();
    }
}

bool mafTreeModel::submit() {
    if (m_CurrentItem->parent() == NULL) {
        m_CurrentItem->setText("root");
        return false;
    }
    m_CurrentItem->data()->setObjectName(m_CurrentItem->text());
    return true;
}

void mafTreeModel::setHierarchy(mafHierarchy *hierarchy) {
    if(m_Hierarchy) {
        disconnect(m_Hierarchy);
    }

    m_Hierarchy = hierarchy;
    connect(m_Hierarchy, SIGNAL(itemAttached(QObject*,QObject*)), this, SLOT(itemAttached(QObject*,QObject*)));
    connect(m_Hierarchy, SIGNAL(itemDetached(QObject*)), this, SLOT(itemDetached(QObject*)));

    initialize();
}

void mafTreeModel::clear() {
    QStandardItemModel::clear();
    m_ItemsList.clear();
    //initialize();
}

void mafTreeModel::itemAttached(QObject *item, QObject *parent) {
    this->selectItemFromData(parent);
    QModelIndex index = this->currentIndex();
    this->insertNewItem(AsChild, item, index);
    emit itemAdded(index);
}

void mafTreeModel::itemDetached(QObject *item) {
    this->selectItemFromData(item);
    this->removeItem(this->currentIndex());
}

mafTreeItem *mafTreeModel::createNewItem(mafTreeItem *parent, QObject *obj, bool done) {
    mafTreeItem *item = new mafTreeItem(obj,done);
    parent->appendRow(item);
    setItem(parent->rowCount()-1, item);
    m_ItemsList.push_back(item);
    return item;
}

void mafTreeModel::selectItem(QModelIndex index) {
    m_CurrentItem = (mafTreeItem *)this->itemFromIndex(index);
}

mafTreeItem *mafTreeModel::insertNewItem(Insert insert,
             QObject *obj, const QModelIndex &index) {
    mafTreeItem *parent;
    if (insert == AtTopLevel) {
        parent = (mafTreeItem *)invisibleRootItem();
    } else {
        if (index.isValid()) {
            parent = (mafTreeItem *) itemFromIndex(index);
            if (!parent)
                return 0;
            if (insert == AsSibling)
                parent = (mafTreeItem *)(parent->parent() ? parent->parent()
                                          : invisibleRootItem());
        } else {
            return 0;
        }
    }
    return createNewItem(parent, obj, false);
}

void mafTreeModel::removeItem(const QModelIndex &index) {
    mafTreeItem *temp = (mafTreeItem *)this->itemFromIndex(index);
    if(temp->parent() == NULL) {
        qDebug() << mafTr("Impossible removing the root");
        return;
    }

    int i = 0, size = temp->rowCount();
    for(; i < size; ++i) {
        removeItem(temp->index().child(i, 0));
    }
    // remove also from the hierarchy?
    m_CurrentItem = (mafTreeItem *)temp->parent();

    removeRow(index.row(), index.parent());

    if(m_CurrentItem->parent() == NULL) {
        setRowCount(m_CurrentItem->rowCount());
    }

    m_ItemsList.removeOne(temp);
}

QModelIndex mafTreeModel::currentIndex() {
    if(m_CurrentItem) {
        return m_CurrentItem->index();
    }

    return QModelIndex();
}

void mafTreeModel::selectItemFromData(QObject *data) {
    QListIterator<mafTreeItem *> it(m_ItemsList);
    while (it.hasNext()) {
        mafTreeItem *ci = it.next();
         QObject *check = ci->data();
         if(check == data) {
             m_CurrentItem = ci;
             return;
         }
     }

    qDebug() << mafTr("Element not found");
}
