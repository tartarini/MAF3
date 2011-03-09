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
#include <QPainter>
#include <QStyle>
#include <QApplication>
#include <QStandardItem>
#include <QPixmap>
#include <QCheckBox>
#include <QLineEdit>


using namespace mafCore;
using namespace mafGUI;

mafTreeItemDelegate::mafTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QWidget *mafTreeItemDelegate::createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();
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
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    //Get lock status
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();
    int lockStatus = objItem->property("lockStatus").toInt();
    switch (lockStatus) {
        case mafCore::mafObjectLockRead: {
                //change item icon
                //Remember standard Icon: QApplication::style()->standardIcon(QStyle::SP_ArrowDown)
                //item->setIcon(lockIcon);
                options.state = QStyle::State_ReadOnly; //set item locked
                QPixmap lockPixmap = QPixmap(":/images/lock_icon.png");
                QRect lockRect = options.rect;
                lockRect.setX(options.rect.x() + 2);

                //Drawind progressBar
                //int progress = index.data().toInt();
                int progress = 20;
                QStyleOptionProgressBar progressBarOption;
                QRect progressRect = options.rect;

                progressRect.setX(options.rect.x() + 40);
                progressBarOption.rect = progressRect;
                progressBarOption.rect.setWidth(option.fontMetrics.width(option.font.toString()));
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
                //Drawing tree item
                QStyledItemDelegate::paint(painter, options, index);
                //Drawing lock icon
                QApplication::style()->drawItemPixmap(painter, lockRect, Qt::AlignLeft, lockPixmap);
            }
            break;
    default:
            QStyledItemDelegate::paint(painter, options, index);
    }
    painter->restore();
}

QSize mafTreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QStyledItemDelegate::sizeHint(option, index);
}


