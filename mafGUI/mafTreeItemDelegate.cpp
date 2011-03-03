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
#include <QSpinBox>
#include <QStyle>
#include <QApplication>
#include <QStandardItem>
#include <QPixmap>
#include <QBitmap>


using namespace mafCore;
using namespace mafGUI;

mafTreeItemDelegate::mafTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void mafTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    //Control checkbox status
    bool checked = index.model()->data(index, Qt::CheckStateRole).toBool();
    QStyleOptionButton check_box_style_option;
    check_box_style_option.state |= QStyle::State_Enabled;
    if (checked) {
        check_box_style_option.state |= QStyle::State_On;
    } else {
        check_box_style_option.state |= QStyle::State_Off;
    }

    check_box_style_option.rect = option.rect.adjusted(1, 1, -1, -1);
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &check_box_style_option, painter, 0);

    //Set icon beside checkbox
    int x = option.rect.x() + 20; //replace 20 with the width of the checkbox.
    int y = option.rect.y();
    int w = option.rect.width() ;
    int h = option.rect.height();

    //Get the VME inside the item
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();

    //Get path to the icon
    QString iconFile;
    QPixmap icon;
    if(!objItem->property("canRead").toBool()) {
        //locked image
        icon = QPixmap(":/images/CLOSE_SASH.bmp");
    } else {
        iconFile = objItem->property("iconFile").toString();
        icon = QPixmap(iconFile);
    }
    if(icon.isNull()) {
       //default image
       icon = QPixmap(":/images/VME_SURFACE.bmp");
    }
    QApplication::style()->drawItemPixmap(painter, QRect(x, y, w, h), Qt::AlignLeft | Qt::AlignVCenter, icon);

    //Get the name of the VME
    QString itemName = objItem->property("objectName").toString();

    //Set VME name beside icon
    int newX = x + icon.rect().width() + 5; //set 2 to create a little space..
    QApplication::style()->drawItemText(painter, QRect(newX, y, w, h), Qt::AlignLeft | Qt::AlignVCenter, QApplication::palette(), option.state, itemName);
}

QSize mafTreeItemDelegate::sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return QSize(20,20);
    }
