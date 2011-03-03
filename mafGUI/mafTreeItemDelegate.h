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

    /// Renders the delegate using the given painter and style option for the item specified by index.
    /*virtual */ void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /// Returns the size needed by the delegate to display the item specified by index.
    /*virtual */ QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;


private:


};

}

#endif // MAFTREEITEMDELEGATE_H
