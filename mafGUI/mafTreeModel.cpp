/*
 *  mafTreeModel.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeModel.h"

using namespace mafCore;
using namespace mafGUI;

mafTreeModel::mafTreeModel(QObject *parent)
    : QStandardItemModel(parent) , m_Hierarchy(NULL), m_CurrentItem(NULL){

}

void mafTreeModel::initialize()
{
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
    }

    int index = 0, size = m_Hierarchy->currentNumberOfChildren();
    for(;index < size; index++) {
        m_Hierarchy->moveTreeIteratorToNthChild(index);
        QObject *obj = m_Hierarchy->currentData();
        mafString name = obj->objectName();
        mafTreeItem *item = new mafTreeItem(obj , false);
        m_CurrentItem->insertRow(index, item);
        //setItem(index, 0, m_CurrentItem);
        m_CurrentItem = item;

        buildModel(false);
        m_Hierarchy->moveTreeIteratorToParent();
        m_CurrentItem = (mafTreeItem *)m_CurrentItem->parent();
    }
}

void mafTreeModel::setHierarchy(mafHierarchy *hierarchy) {
    m_Hierarchy = hierarchy;
    initialize();
}

void mafTreeModel::clear()
{
    QStandardItemModel::clear();
    //initialize();
}

mafTreeItem *mafTreeModel::createNewItem(mafTreeItem *root,
        QObject *obj, bool done)
{
    mafTreeItem *item = new mafTreeItem(obj,done);
    root->appendRow(item);

    return item;
}


mafTreeItem *mafTreeModel::insertNewItem(Insert insert,
             QObject *obj, const QModelIndex &index)
{
    mafTreeItem *parent;
    if (insert == AtTopLevel)
        parent = (mafTreeItem *)invisibleRootItem();
    else {
        if (index.isValid()) {
            parent = (mafTreeItem *) itemFromIndex(index);
            if (!parent)
                return 0;
            if (insert == AsSibling)
                parent = (mafTreeItem *)(parent->parent() ? parent->parent()
                                          : invisibleRootItem());
        }
        else
            return 0;
    }
    return createNewItem(parent, obj, false);
}

QModelIndex mafTreeModel::currentIndex() {
    if(m_CurrentItem) {
        return m_CurrentItem->index();
    }

    return QModelIndex();
}
