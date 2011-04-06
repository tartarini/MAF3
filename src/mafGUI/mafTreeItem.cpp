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
    bool canVisualize = false;
    QString nodeName(obj->metaObject()->className()); ;
    if (nodeName == "mafResources::mafSceneNode"){
        //set icon from VME
        canVisualize = obj->property("canVisualize").toBool();
        if (canVisualize) {
            setCheckState(done ? Qt::Checked : Qt::Unchecked);
            this->flags() |= Qt::ItemIsUserCheckable;
        }
    }
    setCheckable(canVisualize);
    m_Data = obj;
}
