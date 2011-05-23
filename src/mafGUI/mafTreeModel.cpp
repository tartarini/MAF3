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
: QStandardItemModel(parent) , m_Hierarchy(NULL), m_CurrentItem(NULL), m_TreeModelStatus(mafTreeModelStatusGenerate) {
}

void mafTreeModel::initialize() {
    REQUIRE(m_Hierarchy != NULL);
    //header to write
    //setHorizontalHeaderLabels(QStringList() << tr("Item/Item/..."));
    for (int column = 1; column < columnCount(); ++column) {
        horizontalHeaderItem(column)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
    }
    
    if(m_TreeModelStatus == mafTreeModelStatusGenerate) {
        buildModel();
        m_TreeModelStatus = mafTreeModelStatusUpdate;
    } else {
        replaceDataModel();
    }
}

QString mafTreeModel::dataHash(QObject *obj) {
    QVariant hash = obj->property("dataHash");
    if(hash.isValid()) { //vme use directly the objectHash while scenenode dataHash which retrieves vme hash
        //is a scenenode!
        return hash.toString();
    } else {
        //is a vme!
        return obj->property("objectHash").toString();
    }
}

void mafTreeModel::buildModel(bool init) {
    //create root
    if(init) {
        m_Hierarchy->moveTreeIteratorToRootNode();
        m_CurrentItem = new mafTreeItem(m_Hierarchy->currentData());
        setItem(0, 0, m_CurrentItem);
        QObject *data = m_Hierarchy->currentData();
        m_ItemsHash.insert(dataHash(data) , m_CurrentItem);
    }
    
    int i = 0, size = m_Hierarchy->currentNumberOfChildren();
    for(;i < size; ++i) {
        m_Hierarchy->moveTreeIteratorToNthChild(i);
        QObject *obj = m_Hierarchy->currentData();
        
        mafTreeItem *item = this->createNewItem(m_CurrentItem, obj);
        m_CurrentItem = item;
        buildModel(false);
        m_Hierarchy->moveTreeIteratorToParent();
        m_CurrentItem = (mafTreeItem *)m_CurrentItem->parent();       
    }
}

void mafTreeModel::replaceDataModel(bool init) {
    QObject *data;
    mafTreeItem *ti;
    if(init) {
        m_Hierarchy->moveTreeIteratorToRootNode();
        data = m_Hierarchy->currentData();
        char *v = data->objectName().toAscii().data();
        ti = m_ItemsHash.value(dataHash(data));
        ti->setObject(data);
        ti->setStatus(mafItemStatusNotCheckable, false);
    }
    
    int i = 0, size = m_Hierarchy->currentNumberOfChildren();
    for(;i < size; ++i) {
        m_Hierarchy->moveTreeIteratorToNthChild(i);
        data = m_Hierarchy->currentData();
        char *v = data->objectName().toAscii().data();
        ti = m_ItemsHash.value(dataHash(data));
        ti->setObject(data);
        
        QVariant vs = data->property("visualizationStatus");
        if(!vs.isValid()) {
            ti->setStatus(mafItemStatusNotCheckable, false); 
        } else {
            ti->setStatus(vs.toUInt(), data->property("visibility").toBool());
        }
        replaceDataModel(false);
        m_Hierarchy->moveTreeIteratorToParent();
    }
}

void mafTreeModel::setHierarchy(mafHierarchy *hierarchy) {
    if(m_Hierarchy) {
        disconnect(m_Hierarchy, SIGNAL(itemAttached(QObject*,QObject*)), this, SLOT(itemAttached(QObject*,QObject*)));
        disconnect(m_Hierarchy, SIGNAL(itemDetached(QObject*)), this, SLOT(itemDetached(QObject*)));
        disconnect(m_Hierarchy, SIGNAL(itemReparent(QObject*,QObject*)), this, SLOT(itemReparent(QObject*,QObject*)));
        disconnect(m_Hierarchy, SIGNAL(clearTree()), this, SLOT(clearModel()));
        disconnect(m_Hierarchy, SIGNAL(destroyed()), this, SLOT(hierarchyDestroyed()));
    }
    
    m_Hierarchy = hierarchy;
    connect(m_Hierarchy, SIGNAL(itemAttached(QObject*,QObject*)), this, SLOT(itemAttached(QObject*,QObject*)));
    connect(m_Hierarchy, SIGNAL(itemDetached(QObject*)), this, SLOT(itemDetached(QObject*)));
    connect(m_Hierarchy, SIGNAL(itemReparent(QObject*,QObject*)), this, SLOT(itemReparent(QObject*,QObject*)));
    connect(m_Hierarchy, SIGNAL(clearTree()), this, SLOT(clearModel()));
    connect(m_Hierarchy, SIGNAL(destroyed()), this, SLOT(hierarchyDestroyed()));
    
    initialize();
}

void mafTreeModel::clearModel() {
    if(m_TreeModelStatus == mafTreeModelStatusGenerate) {
        QStandardItemModel::clear();
        m_ItemsHash.clear();
    }
}

void mafTreeModel::itemAttached(QObject *item, QObject *parent) {
    // @TODO here set the code because if a view is present, need to attach an item
    // with a scenenode (check with Roberto)
    
    if (parent != NULL) {
        QModelIndex index = this->indexFromData(parent);
        this->insertNewItem(AsChild, item, index);
        emit itemAdded(index);
    }
    ((mafTreeItem*)this->item(0,0))->data()->setProperty("objectName", "pippi");
    QModelIndex index = this->indexFromData(parent);
    //QStandardItem *itema = itemFromIndex(index);
    emit dataChanged(index, index);
}

void mafTreeModel::itemDetached(QObject *item) {
    mafTreeItem *temp = (mafTreeItem *)this->itemFromIndex(this->indexFromData(item));
    bool removed = this->removeRows(temp->index().row(), 1, this->indexFromData(item).parent());
}

mafTreeItem *mafTreeModel::createNewItem(mafTreeItem *parent, QObject *obj) {
    mafTreeItem *item = new mafTreeItem(obj);
    parent->appendRow(item);
    //Update check state
    QVariant vs = obj->property("visualizationStatus"); 
    QVariant v = obj->property("visibility");
    // checkable means that a view at least exists
    // see mafTreeItem: mafSceneNode that can be visible, will set Checkable State to True
    // while in any other case (mafScenenode invisible or other data type like vme)
    // it doesn't matter any visualization
    if(v.isValid() && vs.isValid()){
        bool visibility = obj->property("visibility").toBool();
        item->setStatus(vs.toUInt(), visibility);
        if(visibility) {
            this->setProperty("visibility", visibility);
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(mafCore::mafObjectBase*, (mafObjectBase*)obj));
            argList.append(mafEventArgument(bool, visibility));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.sceneNodeShow", mafEventTypeLocal, &argList);
        }
    } else {
        item->setStatus();
    }
    
    m_ItemsHash.insert(dataHash(obj) , item);
    return item;
}

void mafTreeModel::hierarchyDestroyed() {
    m_Hierarchy = NULL;
}


void mafTreeModel::itemReparent(QObject *obj, QObject *parent) {
    QModelIndex newParentIndex = this->indexFromData(parent);
    mafTreeItem * newParent = (mafTreeItem *)this->itemFromIndex(newParentIndex);
    QModelIndex objectIndex = this->indexFromData(obj);
    mafTreeItem * child = (mafTreeItem *)this->itemFromIndex(objectIndex);
    QModelIndex oldParentIndex = objectIndex.parent();
    mafTreeItem * oldParent = (mafTreeItem *)this->itemFromIndex(oldParentIndex);

    int rows = newParent->rowCount();
    mafTreeItem *newItem = new mafTreeItem(obj);
    newItem->setStatus(child->status(), child->checkState());
    newParent->appendRow(newItem);
    m_ItemsHash.insert(dataHash(obj), newItem);

    char *v = obj->objectName().toAscii().data();
    int i=0, size = child->rowCount();
    for(;i<size;i++) {
        itemReparent(((mafTreeItem *)child->child(i))->data(), newItem->data());
    }
}

void mafTreeModel::selectItem(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);
    if(selected.indexes().count() == 0) {
        return;
    }
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
    
    //mafTreeItem *oldItem = (mafTreeItem *)this->itemFromIndex(indexFromData(obj));
    mafTreeItem *newItem = createNewItem(parent, obj);
    /*if (oldItem) {
        int i = 0, size = oldItem->rowCount();
        for(; i < size; ++i) {
            QObject *child = ((mafTreeItem *)oldItem->child(i))->data();
            insertNewItem(AsChild, child, newItem->index());
        }
    }*/
}

bool mafTreeModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (parent.isValid()) {
        return QStandardItemModel::removeRows(row, count, parent);
    }
    return false;
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
    
    m_ItemsHash.remove(dataHash(temp->data()));
}

QModelIndex mafTreeModel::currentIndex() {
    if(m_CurrentItem) {
        return m_CurrentItem->index();
    }
    
    return QModelIndex();
}

QModelIndex mafTreeModel::indexFromData(QObject *data) {
    if (data != NULL) {
        mafTreeItem *ti = m_ItemsHash.value(dataHash(data),NULL);
        if(ti == NULL) {
            return QModelIndex();
        }
    return ti->index();
    }
}


