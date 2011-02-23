/*
 *  mafHierarchy.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 24/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafHierarchy.h"
#include "mafObjectFactory.h"

using namespace mafCore;

mafHierarchy::mafHierarchy(const mafString code_location) : mafObjectBase(code_location), m_Tree(NULL) {
    //create Tree
    m_Tree = new mafTree<QObject *>();
    m_TreeIterator = m_Tree->root();
}

mafHierarchy::~mafHierarchy() {
    mafDEL(m_Tree);
}

void mafHierarchy::addHierarchyNode(QObject* node, QObject *parentNode) {
    if(parentNode == NULL) {
        if(m_TreeIterator.node() == NULL) {
            m_TreeIterator = m_Tree->insert(node);
        } else {
            int numberOfChildren = m_TreeIterator.node()->m_children.size();
            m_TreeIterator = m_Tree->insert(m_TreeIterator, numberOfChildren, node);
        }
    } else {
        moveTreeIteratorToNode(parentNode);
        int numberOfChildren = m_TreeIterator.node()->m_children.size();
        m_TreeIterator = m_Tree->insert(m_TreeIterator, numberOfChildren, node);
    }
    emit itemAttached(node, parentNode);
}

void mafHierarchy::removeCurrentHierarchyNode(bool free_memory) {
    mafTree<QObject *>::iterator temp_iterator = m_Tree->parent(m_TreeIterator);

    mafTree<QObject *> cutTree;
    cutTree = m_Tree->cut(m_TreeIterator);
    for (mafTree<QObject *>::prefix_iterator i = cutTree.prefix_begin(); i != cutTree.prefix_end(); ++i) {
        mafTreeNode<QObject *> *n = i.simplify().node();
        if(n->m_data && free_memory) {
           delete n->m_data;
           n->m_data = NULL;
        }
    }
    m_Tree->erase(m_TreeIterator);
    m_TreeIterator = temp_iterator;
}

void mafHierarchy::removeHierarchyNode(QObject *node, bool free_memory) {
    REQUIRE(node != NULL);

    // Move the iterator to the node to remove.
    moveTreeIteratorToNode(node);
    // notify the observer that a node is going to be detached.
    emit itemDetached(node);
    // Remove the node from the hierarchy => the item will be deleted !!
    removeCurrentHierarchyNode(free_memory);
}

void mafHierarchy::moveTreeIteratorToParent() {
    m_TreeIterator = m_Tree->parent(m_TreeIterator);
}

void mafHierarchy::moveTreeIteratorToNthChild(unsigned int childIndex) {
    REQUIRE(m_Tree->children(m_TreeIterator) > childIndex);

    m_TreeIterator = m_Tree->child(m_TreeIterator, childIndex);
}

unsigned int mafHierarchy::currentNumberOfChildren() {
    return m_Tree->children(m_TreeIterator);
}

unsigned int mafHierarchy::numberOfChildren(QObject *node) {
    moveTreeIteratorToNode(node);
    return currentNumberOfChildren();
}

void mafHierarchy::moveTreeIteratorToNode(QObject *node) {
    REQUIRE(node != NULL);

    for (mafTree<QObject *>::prefix_iterator i = m_Tree->prefix_begin(); i != m_Tree->prefix_end(); ++i) {
        mafTreeNode<QObject *> *n = i.simplify().node();
        if(n->m_data == node) {
            m_TreeIterator =  i.simplify();
            break;
        }
    }
}

void mafHierarchy::moveTreeIteratorToRootNode() {
    m_TreeIterator = m_Tree->root();
}

void mafHierarchy::clear() {
    mafTree<QObject *>::prefix_iterator i = m_Tree->prefix_begin();
    mafTree<QObject *>::prefix_iterator iterEnd = m_Tree->prefix_end();
    for (; i != iterEnd; ++i) {
        mafTreeNode<QObject *> *n = i.simplify().node();
        if(n->m_data) {
            delete n->m_data;
            n->m_data = NULL;
        }
    }
    m_TreeIterator = m_Tree->root();
    m_Tree->erase(m_TreeIterator);
    m_TreeIterator.null();
}

void mafHierarchy::printInformation(mafString &string) {
    for (mafTree<QObject *>::prefix_iterator i = m_Tree->prefix_begin(); i != m_Tree->prefix_end(); ++i) {
        mafTreeNode<QObject *> *n = i.simplify().node();
        QObject *obj = n->m_data;
        if(obj != NULL) {
            string.append(obj->objectName());
            string.append("\n");
        }
    }
}
