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
#include "mafObjectBase.h"
#include "mafTree.hpp"

namespace mafCore {

// Class forwarding list

/**
Class name: mafHierarchy
This class represents the base class to manage objects' hierarchy. A hierarchy is a collection of nodes in a tree structure.
A node may have many children but often only a single parent, with the effect of a parent applied to all its child nodes.
The tree is managed towards the class stlplus::ntree, included in MAF3 Foundation in utility section.
*/
class MAFCORESHARED_EXPORT mafHierarchy : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafHierarchy(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafHierarchy();

    /// Add a node to the tree.
    void addHierarchyNode(QObject *node, QObject *parentNode = 0);

    /// Remove the current selected node from the tree.
    void removeCurrentHierarchyNode();

    /// Remove the current selected node from the tree.
    void removeHierarchyNode(QObject *node);

    /// Move the tree iterator to the parent of the current position pointed
    void moveTreeIteratorToParent();

    /// Move the tree iterator to the given node
    void moveTreeIteratorToNode(QObject *node);

    /// Move the tree iterator to the indexed child of the current position pointed
    void moveTreeIteratorToNthChild(unsigned int childIndex = 0);

    /// reterun the number of children of the current node
    unsigned int currentNumberOfChildren();

    /// Move the tree iterator to the root.
    void moveTreeIteratorToRootNode();

    /// Fill the parameter string with a text representation of the tree.
    void printInformation(mafString &string);

    /// retrieve current data inside the node
    QObject *currentData() { return m_TreeIterator.node()->m_data; }

    /// Remove all the nodes and delete them.
    void clear();

private:
    mafTree<QObject *> *m_Tree; ///< tree variable which hosts mafSceneNode elements
    mafTree<QObject *>::iterator m_TreeIterator; ///< tree iterator variable which point the current node
};

} //namespace mafCore

#endif // MAFHIERARCHY_H
