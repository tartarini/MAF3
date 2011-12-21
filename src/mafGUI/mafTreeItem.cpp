/*
 *  mafTreeItem.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeItem.h"

using namespace mafGUI;

mafTreeItem::mafTreeItem(QObject *obj) : QStandardItem(obj->objectName()), m_Data(obj) {
    setIcon(QIcon(obj->property("iconFile").toString()));
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|
             Qt::ItemIsEditable);
    setStatus(mafItemStatusNotCheckable, false);
}

void mafTreeItem::setObject(QObject *data) {
    m_Data = data;
}

void mafTreeItem::setStatus(unsigned int status, bool active) {
    m_Status = (mafItemStatus)status;
    switch(status) {
        case mafItemStatusCheckable:
            setCheckable(true);
            setCheckState(active ? Qt::Checked : Qt::Unchecked);
            break;
        case mafItemStatusNotCheckable:
        {
            setCheckable(false);
            //due to a bug in Qt it is not disable CheckStateRole
            this->QStandardItem::setData(QVariant(), Qt::CheckStateRole);
        }
            break;
        case mafItemStatusMutexCheckable:
            qDebug() << "MUTEX STATUS";
            break;
    }
}

mafItemStatus mafTreeItem::status() const {
    return m_Status;
}
