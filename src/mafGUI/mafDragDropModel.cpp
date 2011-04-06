/*
 *  mafDragDropModel.cpp
 *  mafGui
 *
 *  Created by Roberto Mucci on 11/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

//#include <QtGui>

#include "mafDragDropModel.h"

#include "mafTreeItem.h"

using namespace mafCore;
using namespace mafGUI;
using namespace mafEventBus;

mafDragDropModel::mafDragDropModel(/*const QStringList &strings,*/ QObject *parent)
    : mafTreeModel()
{
}

bool mafDragDropModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent) {

    if (action == Qt::IgnoreAction)
        return true;

    mafTreeItem *itemParent;
    if (!parent.isValid()) {
        QString root("root");
        itemParent = (mafTreeItem *)this->findItems(root).at(0);
    } else {
        itemParent = (mafTreeItem *)((QStandardItemModel *)parent.model())->itemFromIndex(parent);
    }
    mafObjectBase *objParent = qobject_cast<mafObjectBase*>(itemParent->data());
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)currentIndex().model())->itemFromIndex(currentIndex());
    mafObjectBase *obj = qobject_cast<mafObjectBase*>(item->data());

    //Notify vme reparent
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
    argList.append(mafEventArgument(mafCore::mafObjectBase *, objParent));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.reparent", mafEventTypeLocal, &argList);
    return true;
}

Qt::ItemFlags mafDragDropModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);
    if (index.isValid()) {
        mafTreeItem *item = (mafTreeItem *)this->itemFromIndex(index);
        if (item->data() != NULL) {
            if (item->data()->objectName().compare("root") == 0) {
                // Not allow Drag of the Root
                return Qt::ItemIsDropEnabled | defaultFlags;
            }
        }
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    } else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList mafDragDropModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

Qt::DropActions mafDragDropModel::supportedDropActions() const
{
    //Only move action. Copy action can be enabled.
    return /*Qt::CopyAction |*/ Qt::MoveAction;
}
