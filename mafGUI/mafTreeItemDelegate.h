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

#ifndef MAFTREEITEMDELEGATE_H
#define MAFTREEITEMDELEGATE_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QObject>



namespace mafGUI {
/**
Class Name: mafTreeItemDelegate

*/

class MAFGUISHARED_EXPORT mafTreeItemDelegate : public QStyledItemDelegate {
Q_OBJECT
public:
    /// Object constructor.
    /*explicit*/ mafTreeItemDelegate(QObject *parent = 0);

    /// Returns the editor to be used for editing the data item with the given index.
    /*virtual */ //QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    /// Sets the contents of the given editor to the data for the item at the given index.
    /*virtual */ void setEditorData( QWidget * editor, const QModelIndex & index ) const;

    /// Gets data from the editor widget and stores it in the specified model at the item index.
    /*virtual */ void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

    /// Renders the delegate using the given painter and style option for the item specified by index.
    /*virtual */ void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /// Returns the size needed by the delegate to display the item specified by index.
    /*virtual */ QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    /// Alert observers about checkbox status.
    void checkBoxStatus(bool status, const QModelIndex &index) const;

public slots:

private:



};

}

#endif // MAFTREEITEMDELEGATE_H
