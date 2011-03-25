/*
 *  mafTreeItemDelegate.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 01/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTreeItemDelegate.h"
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

mafTreeItemDelegate::mafTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent), m_isSceneNode(false ){
}

QWidget *mafTreeItemDelegate::createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QObject *objItem = objFromIndex(index);
    int lockStatus = objItem->property("lockStatus").toInt();
    QWidget *editor = NULL;
    // Not allow VME name editing to locked item.
    if (lockStatus == mafCore::mafObjectLockNone) {
        editor = QStyledItemDelegate::createEditor(parent, option, index);
    }
    return editor;
}

void mafTreeItemDelegate::setEditorData( QWidget * editor, const QModelIndex & index ) const {
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *textEditor = static_cast<QLineEdit*>(editor);
    textEditor->setText(text);
}

void mafTreeItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const {
    //Get new VME name.
    QLineEdit *textEditor = static_cast<QLineEdit*>(editor);
    QString value = textEditor->text();
    model->setData(index, value, Qt::EditRole);

    //Set new VME name to the VME property.
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();
    objItem->setProperty("objectName", value );
}

void mafTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);
    QPixmap iconPixmap;

    //Get lock status
    QObject *objItem = objFromIndex(index);
    uint lockStatus = objItem->property("lockStatus").toUInt();
    if (lockStatus & mafCore::mafObjectLockNone) {
       item->setIcon(QIcon(objItem->property("iconFile").toString()));
    }
    if (lockStatus & mafCore::mafObjectLockProgres)  {
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

bool mafTreeItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    bool result = QStyledItemDelegate::editorEvent(event, model, option, index);
    QObject *sceneNode = objFromIndex(index);
    //If item is a SceneNode, set visibility property to item checked
    if (sceneNode->metaObject()->className() == "mafResources::mafSceneNode") {
        m_isSceneNode = true;
        QVariant value = index.data(Qt::CheckStateRole);
        if (!value.isValid())
            return result;
        bool state = (static_cast<Qt::CheckState>(value.toInt())) ? true : false;
        sceneNode->setProperty("visibility", state);
    } else {
        m_isSceneNode = false;
    }
    return result;
}

QObject *mafTreeItemDelegate::objFromIndex(const QModelIndex &index) const {
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    return item->data();
}
