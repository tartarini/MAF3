/*
 *  mafTreeItemDelegate.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 01/03/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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
This class is responsible for display and editing items of the tree model.
*/

class MAFGUISHARED_EXPORT mafTreeItemDelegate : public QStyledItemDelegate {
Q_OBJECT
public:
    /// Object constructor.
    /*explicit*/ mafTreeItemDelegate(QObject *parent = 0);

    /// Returns the widget used to edit the item specified by index for editing.
    /*virtual*/ QWidget *createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    /// Sets the contents of the given editor to the data for the item at the given index.
    /*virtual */ void setEditorData( QWidget * editor, const QModelIndex & index ) const;

    /// Gets data from the editor widget and stores it in the specified model at the item index.
    /*virtual */ void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

    /// Renders the delegate using the given painter and style option for the item specified by index.
    /*virtual */ void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /// set the lock for all the items globally.
    static void setGlobalLock(bool globalLock);
    
    /// return global lock for selection.
    static bool globalLock();

protected:
    /// Return the QObject corrisponding to index.
    QObject *objFromIndex(const QModelIndex &index) const;

    static bool m_GlobalLock;
    mutable bool m_IsEditing;
};

}

#endif // MAFTREEITEMDELEGATE_H
