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
#include <QCheckBox>
#include <QLineEdit>
#include <QPixmapCache>


using namespace mafCore;
using namespace mafGUI;

mafTreeItemDelegate::mafTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
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
    QPalette::ColorRole textColorRole = QPalette::NoRole;
    QString vmeIconFile;
    QPixmap vmeIcon;

    //Check if model is a mafSceneNode and set property visibility.
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();

    //Load icon of the VME
    vmeIconFile = objItem->property("iconFile").toString();
    vmeIcon = QPixmap(vmeIconFile);
    if(vmeIcon.isNull()) {
        //default image
        vmeIcon = QPixmap(":/images/questionMark.png");
    }

    //Get lock status
    int lockStatus = objItem->property("lockStatus").toInt();
    switch (lockStatus) {
        case mafCore::mafObjectLockNone: {
            //Draw CheckBox
            QStyleOptionButton checkBoxOption;
            checkBoxOption.rect = option.rect.adjusted(1, 1, -1, -1);
            checkBoxOption.state |= QStyle::State_Enabled; //diable in some cases.

            //Control checkbox status
            bool checkStatus = index.model()->data(index, Qt::CheckStateRole).toBool();
            if (checkStatus) {
                checkBoxOption.state |= QStyle::State_On;
            } else {
                checkBoxOption.state |= QStyle::State_Off;
            }
            QString objType = objItem->metaObject()->className();
            if(objType.compare("mafResources::mafSceneNode") == 0){
                bool visibility = objItem->property("visibility").toBool();
                if(visibility != checkStatus) {
                    objItem->setProperty("visibility", checkStatus);
                }
            }
            QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter, 0);
          break;
      }
        case mafCore::mafObjectLockRead: {
            //and case mafCore::mafObjectLockWrite:
            //Draw lock Icon.
            QPixmap lockIcon;
            lockIcon = QPixmap(":/images/lock_icon.png");
            // set Label in grey
            textColorRole = QPalette::Mid;
            //Draw Icon.
            QApplication::style()->drawItemPixmap(painter, option.rect, Qt::AlignLeft | Qt::AlignVCenter, lockIcon);
            // set vmeIcon to greyscale
            vmeIcon = this->toGrayScale(vmeIcon);
        break;
        }
    }
    //Set icon beside checkbox
    int x = option.rect.x() +20; //replace 20 with the width of the checkbox.
    int y = option.rect.y();
    int w = option.rect.width() ;
    int h = option.rect.height();

    //Draw vmeIcon.
    QApplication::style()->drawItemPixmap(painter, QRect(x, y, w, h), Qt::AlignLeft | Qt::AlignVCenter, vmeIcon);

    //Set VME name beside vmeIcon
    int newX = x + vmeIcon.rect().width() + 5; //set 5 to create a little space..

    //Get the name of the Object
    QString itemName = objItem->property("objectName").toString();
    //Draw VME name.
    QApplication::style()->drawItemText(painter, QRect(newX, y, w, h), Qt::AlignLeft | Qt::AlignVCenter, QApplication::palette(), option.state, itemName, textColorRole);
}

QSize mafTreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return QSize(20,20);
}

QPixmap mafTreeItemDelegate::toGrayScale(QPixmap icon) const {
    QPixmap pix = icon;
    QImage image = icon.toImage();
    QRgb col;
    int gray;
    int width = icon.width();
    int height = icon.height();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            col = image.pixel(i, j);
            gray = qGray(col);
            image.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }
    pix = pix.fromImage(image);
    return pix;
}


