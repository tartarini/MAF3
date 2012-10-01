/*
 *  mafTreeItemSceneNodeDelegate.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 01/03/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTREEITEMSCENENODEDELEGATE_H
#define MAFTREEITEMSCENENODEDELEGATE_H

// Includes list
#include "mafGUIDefinitions.h"
#include "mafTreeItemDelegate.h"
#include <QModelIndex>
#include <QObject>



namespace mafGUI {
/**
Class Name: mafTreeItemSceneNodeDelegate
This class is responsible for display and editing items of the tree model.
*/

class MAFGUISHARED_EXPORT mafTreeItemSceneNodeDelegate : public mafTreeItemDelegate {
Q_OBJECT
public:
    /// Object constructor.
    /*explicit*/ mafTreeItemSceneNodeDelegate(QObject *parent = 0);

    /// Gets data from the editor widget and stores it in the specified model at the item index.
    /*virtual */ void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

    /// Whenever an event occurs, this function is called with the event model option and the index that corresponds to the item being edited.
    /*virtual*/ bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index );
};

}

#endif // MAFTREEITEMSCENENODEDELEGATE_H
