/*
 *  mafTreeItem.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeItem.h"

using namespace mafGUI;

mafTreeItem::mafTreeItem(QObject *obj, bool done) : QStandardItem(obj->objectName()) {
    setIcon(QIcon(obj->property("iconFile").toString()));
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|
             Qt::ItemIsEditable);
    m_Data = obj;
    setStatus(mafItemStatusNotCheckable, false);
}

void mafTreeItem::setData(QObject *data) {
    m_Data = data;
}

void mafTreeItem::setStatus(unsigned int status, bool active) {
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
