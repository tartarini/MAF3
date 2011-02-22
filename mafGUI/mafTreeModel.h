/*
 *  mafTreeModel.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTREEMODEL_H
#define MAFTREEMODEL_H

// Includes list
#include "mafGUIDefinitions.h"
#include "mafTreeItem.h"
#include <mafHierarchy.h>

namespace mafGUI {

/**
Class Name: mafTreeModel
It represents the interface to the data model which need to be represented by
a tree view or a list view. It is used to interface with mafHerarchy which is a template
tree structure.
*/
class MAFGUISHARED_EXPORT mafTreeModel : public QStandardItemModel {
    Q_OBJECT

public:
    /// enum for the type of insertion
    enum Insert {AtTopLevel, AsSibling, AsChild};

    /// Object Constructor
    mafTreeModel(QObject *parent=0);

    /// return the current hierarchy which is hidden by this interface
    mafCore::mafHierarchy *hierarchy() const { return m_Hierarchy; }

    /// set the current hierrchy to a new hierarchy
    void setHierarchy(mafCore::mafHierarchy *hierarchy);

    /// clear the model
    void clear();

    /// insert a new item in the model
    mafTreeItem *insertNewItem(Insert insert, QObject *obj, const QModelIndex &index);

    /// remove item from the model
    void removeItem(const QModelIndex &index);

    /// utility function. This function is useful in test phase for select a tree item from data inside
    void selectItemFromData(QObject *data);

    /// retrieve index of current element
    QModelIndex currentIndex();

public slots:
    /// select the object from the data, return true if selected, false otherwise.
    void selectItem(QModelIndex index);

    /*virtual*/ bool submit();

private slots:
    /// Slot called when a new item has been attached to the tree hierarchy.
    void itemAttached(QObject *item, QObject *parent);

    /// Slot called when an item is going to be removed from the tree hierarchy
    void itemDetached(QObject *item);

signals:
    void itemAdded(QModelIndex index);
    //void save();

private:
    /// initialization of the header which will be visualized by a view
    void initialize();

    /// recursive function which is able to reconstruct from a hierarchy, the current model
    void buildModel(bool init = true);

    /// create new item starting by a parent item
    mafTreeItem *createNewItem(mafTreeItem *parent, QObject *obj, bool checked);

    mafCore::mafHierarchy *m_Hierarchy; ///< hierarchy from which construct the model
    mafTreeItem *m_CurrentItem; ///< current item of the model

    mafList<mafTreeItem *> m_ItemsList; ///< variable useful for rapid iteration between mafTreeItem
};

} //end namespace

#endif // MAFTREEMODEL_H
