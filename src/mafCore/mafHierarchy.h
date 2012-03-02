/*
 *  mafHierarchy.h
 *  mafCore
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 24/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFHIERARCHY_H
#define MAFHIERARCHY_H

// Includes list
#include "mafObject.h"
#include "mafTree.hpp"

namespace mafCore {

// Class forwarding list

/**
Class name: mafHierarchy
This class represents the base class to manage objects' hierarchy. A hierarchy is a collection of nodes in a tree structure.
A node may have many children but often only a single parent, with the effect of a parent applied to all its child nodes.
The tree is managed towards the class stlplus::ntree, included in MAF3 Foundation in utility section.
*/
class MAFCORESHARED_EXPORT mafHierarchy : public mafObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public Q_SLOTS:
    /// Move the tree iterator to the given node
    void moveTreeIteratorToNode(QObject *node);

Q_SIGNALS:
        /// Signal emitted when a new item is attached to the hierarchy.
        void itemAttached(QObject *item, QObject *parent);

        /// Signal emitted when an item is removed form the hierarchy.
        void itemDetached(QObject *item);

        /// Signal emitted to replace root item.
        void rootReplaced();

        /// Signal emitted when an item is reparent to another item.
        void itemReparent(QObject *item, QObject *parent);

        /// Signal emitted when the tree is cleared.
        void clearTree();

public:
    /// Object constructor.
    mafHierarchy(const QString code_location = "");

    /// Add a node to the tree.
    void addHierarchyNode(QObject *node, QObject *parentNode = 0);

    /// Remove the current selected node from the tree. The current node will be deleted.
    void removeCurrentHierarchyNode();

    /// Remove the current selected node from the tree. The node will be deleted.
    void removeHierarchyNode(QObject *node);

    /// Reparent node to parentNode.
    void reparentHierarchyNode(QObject *node, QObject *parentNode);

    /// Move the tree iterator to the parent of the current position pointed
    void moveTreeIteratorToParent();
    
    /// if the node has't parent (is root) return false, otherwise true.
    bool hasParent();

    /// Move the tree iterator to the indexed child of the current position pointed
    void moveTreeIteratorToNthChild(unsigned int childIndex = 0);

    /// reterun the number of children of the current node
    unsigned int currentNumberOfChildren();

    /// Return the number of sub-tree items
    unsigned int numberOfChildren(QObject *node);

    /// Move the tree iterator to the root.
    void moveTreeIteratorToRootNode();

    /// Fill the parameter string with a text representation of the tree.
    void printInformation(QString &string);

    /// retrieve current data inside the node
    QObject *currentData() { return m_TreeIterator.node()->m_data; }

    /// Remove all the nodes and delete them.
    void clear();

    /// set current iterator.
    void setIterator(mafTree<QObject *>::iterator iterator);

    /// Return current iterator.
    mafTree<QObject *>::iterator iterator();

    /// Return the instance of the resource's status. The caller has to delete the allocated memory he asked.
    /*virtual*/ mafCore::mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization pourposes.*/
    /*virtual*/ void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

protected:
    /// Object destructor.
    /* virtual */ ~mafHierarchy();

private:
    /// Check if a node can be added as child of the given parent or no.
    bool canAddNodeToParent(QObject *node, QObject *parent);

    mafTree<QObject *> *m_Tree; ///< tree variable which hosts mafSceneNode elements
    mafTree<QObject *>::iterator m_TreeIterator; ///< tree iterator variable which point the current node
};
typedef mafHierarchy * mafHierarchyPointer;

} // namespace mafCore

// Type registered below can be substituted (or integrated) with this one:
Q_DECLARE_METATYPE(mafCore::mafHierarchyPointer);
#endif // MAFHIERARCHY_H
