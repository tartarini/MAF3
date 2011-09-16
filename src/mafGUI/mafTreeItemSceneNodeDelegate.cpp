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

mafTreeItemSceneNodeDelegate::mafTreeItemSceneNodeDelegate(QObject *parent) : QStyledItemDelegate(parent), m_isSceneNode(false ){
}

QWidget *mafTreeItemSceneNodeDelegate::createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QObject *objItem = objFromIndex(index);
    int lockStatus = objItem->property("lockStatus").toInt();
    QWidget *editor = NULL;
    // Not allow VME name editing to locked item.
    if (lockStatus == mafCore::mafObjectLockNone) {
        editor = QStyledItemDelegate::createEditor(parent, option, index);
    }
    return editor;
}

void mafTreeItemSceneNodeDelegate::setEditorData( QWidget * editor, const QModelIndex & index ) const {
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *textEditor = static_cast<QLineEdit*>(editor);
    textEditor->setText(text);
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

void mafTreeItemSceneNodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
   if(!index.isValid()) {
     return;
   }
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);
    QPixmap iconPixmap;

    QObject *objItem = objFromIndex(index);
    QString value = objItem->property("VMEName").toString();
    ((QStandardItemModel *)index.model())->setData(index, value, Qt::DisplayRole);

    int c = index.row();

    //Get lock status
    uint lockStatus = objItem->property("lockStatus").toUInt();
    if (lockStatus & mafCore::mafObjectLockNone) {
       item->setIcon(QIcon(objItem->property("iconFile").toString()));
    }
    if (lockStatus & mafCore::mafObjectLockProgress)  {
        //Drawing Progress bar (first to be covered by item label).
        int checkSpacer = 5;
        if (item->isCheckable()) {
            checkSpacer = 25;
        }

        //Get progress bar value from "progressStatus" property.
        int progress = objItem->property("progressStatus").toInt();
        QStyleOptionProgressBar progressBarOption;
        QRect rectProgress = options.rect;
        rectProgress.setX(options.rect.x() + options.decorationSize.width() + checkSpacer);
        int x = options.fontMetrics.averageCharWidth() * options.text.size();
        rectProgress.setWidth(qMax(x + 60, 100));
        progressBarOption.rect = rectProgress;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignLeft;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;

        painter->save();
        painter->setOpacity(0.8);
        //Drawing Progress bar (first to be covered by item label).
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        painter->restore();
        //I can set an icon for Uploading/Donloading
    }

    if (lockStatus & mafCore::mafObjectLockRead || lockStatus & mafCore::mafObjectLockWrite)  {
        //Set lock icon and diable item
        iconPixmap = QPixmap(":/images/lock_icon.png");
        item->setIcon(QIcon(iconPixmap));
        options.state = QStyle::State_ReadOnly; //set item locked
    }

    //Drawing tree item
    QStyledItemDelegate::paint(painter, options, index);
}

bool mafTreeItemSceneNodeDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    bool result = QStyledItemDelegate::editorEvent(event, model, option, index);
    QObject *sceneNode = objFromIndex(index);
    //If item is a SceneNode, set visibility property to item checked
    m_isSceneNode = true;
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

QObject *mafTreeItemSceneNodeDelegate::objFromIndex(const QModelIndex &index) const {
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    return item->data();
}
