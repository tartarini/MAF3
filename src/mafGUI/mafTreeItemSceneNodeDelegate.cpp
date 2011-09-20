/*
 *  mafTreeItemSceneNodeDelegate.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 01/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeItemSceneNodeDelegate.h"
#include "mafTreeItem.h"
#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <QPainter>
#include <QStyle>
#include <QApplication>
#include <QStandardItem>
#include <QPixmap>
#include <QCheckBox>
#include <QLineEdit>


using namespace mafCore;
using namespace mafGUI;
using namespace mafEventBus;

mafTreeItemSceneNodeDelegate::mafTreeItemSceneNodeDelegate(QObject *parent) : mafTreeItemDelegate(parent) {
}

void mafTreeItemSceneNodeDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const {
    //Get new VME name.
    QLineEdit *textEditor = static_cast<QLineEdit*>(editor);
    QString value = textEditor->text();
    model->setData(index, value, Qt::EditRole);

    //Set new VME name to the VME property.
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();
    objItem->setProperty("VMEName", value);
}

bool mafTreeItemSceneNodeDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    bool result = QStyledItemDelegate::editorEvent(event, model, option, index);
    QObject *sceneNode = objFromIndex(index);

    //If item is a SceneNode, set visibility property to item checked
    QVariant value = index.data(Qt::CheckStateRole);
    if (!value.isValid())
        return result;
    bool state = (static_cast<Qt::CheckState>(value.toInt())) ? true : false;
    bool visibility = sceneNode->property("visibility").toBool();
    if (visibility != state) {
        //sceneNode->setProperty("visibility", state);
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase*, (mafObjectBase*)sceneNode));
        argList.append(mafEventArgument(bool, state));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.sceneNodeShow", mafEventTypeLocal, &argList);
    }

    return result;
}
