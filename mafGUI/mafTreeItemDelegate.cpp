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
    // Not allow VME name editing to locked item.
    if (lockStatus == mafCore::mafObjectLockNone) {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    } else {
        return NULL;
    }
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
    bool isLocked = false;
    QRect mainRect = option.rect; //Rect of the tree item
    QPalette::ColorRole textColorRole = QPalette::NoRole;
    int alignmentFlag =  Qt::AlignLeft;
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
            //Drawing CheckBox
            QStyleOptionButton checkBoxOption;
            checkBoxOption.rect = mainRect.adjusted(1, 1, -1, -1);
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
            alignmentFlag |=  Qt::AlignVCenter;
          break;
      }
        case mafCore::mafObjectLockRead: {
            //and case mafCore::mafObjectLockWrite:
            isLocked = true;
            //Drawing lock Icon.
            QPixmap lockIcon;
            lockIcon = QPixmap(":/images/lock_icon.png");
            // set Label in grey
            textColorRole = QPalette::Mid;
            alignmentFlag |= Qt::AlignTop;
            //Drawing Icon.
            mainRect.setY(mainRect.y()+3);
            QApplication::style()->drawItemPixmap(painter, mainRect, alignmentFlag, lockIcon);
            // set vmeIcon to greyscale
            vmeIcon = this->toGrayScale(vmeIcon);

            ///////  progress bar code ///////////
            //int progress = index.data().toInt();
            int progress = 20;
            QStyleOptionProgressBar progressBarOption;
            progressBarOption.rect = mainRect;
            progressBarOption.rect.setY(lockIcon.height() -5);
            progressBarOption.rect.setWidth(200);
            progressBarOption.rect.setTopLeft(mainRect.center());
            progressBarOption.rect.setBottomLeft(mainRect.bottomLeft());
            progressBarOption.minimum = 0;
            progressBarOption.maximum = 100;
            progressBarOption.textAlignment = Qt::AlignCenter;
            progressBarOption.progress = progress;
            progressBarOption.text = QString::number(progress) + "%";
            progressBarOption.textVisible = true;
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
            ////////////////////////////////////////
        break;
        }
    }
    //Set icon beside checkbox
    QRect vmeIconRect;
    vmeIconRect = mainRect;
    vmeIconRect.setX(mainRect.x() +20);
    //Drawing vmeIcon.
    QApplication::style()->drawItemPixmap(painter, vmeIconRect, alignmentFlag, vmeIcon);

    //Set VME name beside vmeIcon
    QRect labelRect;
    labelRect = mainRect;
    labelRect.setX(vmeIconRect.x() + vmeIcon.rect().width() + 10);

    //Get the name of the Object
    QString itemName = objItem->property("objectName").toString();
    painter->save();
    if (option.state & QStyle::State_Selected && !isLocked) {
        //Highlight selected VME
        QRect paintRect = labelRect;
        paintRect.setX(labelRect.x() - 5);//expande a little painted area.
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::darkGray);
        painter->drawRect(paintRect);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(option.palette.highlightedText().color());
        painter->setBrush(option.palette.highlightedText());
    }

    //Drawing VME name.
    QApplication::style()->drawItemText(painter, labelRect, alignmentFlag, QApplication::palette(), option.state, itemName, textColorRole);
    painter->restore();
}

QSize mafTreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    mafTreeItem *item = (mafTreeItem *)((QStandardItemModel *)index.model())->itemFromIndex(index);
    QObject *objItem = item->data();
    int lockStatus = objItem->property("lockStatus").toInt();
    if (lockStatus == mafCore::mafObjectLockRead) {
        return QSize(20, 40);
        } else {
        return QSize(20,20);
    }
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


