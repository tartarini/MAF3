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

    /// Move the tree iterator to the given node
    void moveTreeIteratorToNode(QObject *node);

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

signals:
    /// Signal emitted when a new item is attached to the hierarchy.
    void itemAttached(QObject *item, QObject *parent);

    /// Signal emitted when an item is removed form the hierarchy.
    void itemDetached(QObject *item);

    /// Signal emitted when an item is reparent to another item.
    void itemReparent(QObject *item, QObject *parent);



protected:
    /// Object destructor.
    /* virtual */ ~mafHierarchy();

private:
    mafTree<QObject *> *m_Tree; ///< tree variable which hosts mafSceneNode elements
    mafTree<QObject *>::iterator m_TreeIterator; ///< tree iterator variable which point the current node
};
typedef mafHierarchy * mafHierarchyPointer;

} // namespace mafCore

// Type registered below can be substituted (or integrated) with this one:
Q_DECLARE_METATYPE(mafCore::mafHierarchyPointer);
#endif // MAFHIERARCHY_H
