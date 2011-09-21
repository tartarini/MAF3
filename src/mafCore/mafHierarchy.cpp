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
#include "mafMemento.h"
#include "mafMementoHierarchy.h"
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;


mafHierarchy::mafHierarchy(const QString code_location) : mafObject(code_location), m_Tree(NULL) {
    //create Tree
    m_Tree = new mafTree<QObject *>();
    m_TreeIterator = m_Tree->root();
}

mafHierarchy::~mafHierarchy() {
    clear();
    delete m_Tree;
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
    
    mafObjectBase *obj = qobject_cast<mafObjectBase *>(node);
    if (obj) {
        obj->retain();
    }
    
    Q_EMIT itemAttached(node, parentNode);
}

void mafHierarchy::removeCurrentHierarchyNode() {
    mafTree<QObject *>::iterator temp_iterator = m_Tree->parent(m_TreeIterator);

    mafTree<QObject *> cutTree;
    cutTree = m_Tree->cut(m_TreeIterator);
    for (mafTree<QObject *>::prefix_iterator i = cutTree.prefix_begin(); i != cutTree.prefix_end(); ++i) {
        mafTreeNode<QObject *> *n = i.simplify().node();
        if(n->m_data) {
            mafObjectBase *obj = qobject_cast<mafObjectBase *>(n->m_data);
            if (obj) {
                obj->release();
            } else {
                delete n->m_data;
            }
            n->m_data = NULL;
        }
    }
    m_Tree->erase(m_TreeIterator);
    m_TreeIterator = temp_iterator;
}

void mafHierarchy::removeHierarchyNode(QObject *node) {
    REQUIRE(node != NULL);

    // Move the iterator to the node to remove.
    moveTreeIteratorToNode(node);
    // notify the observer that a node is going to be detached.
    Q_EMIT itemDetached(node);
    // Remove the node from the hierarchy => the item will be deleted !!
    removeCurrentHierarchyNode();
}

void mafHierarchy::reparentHierarchyNode(QObject *node, QObject *parentNode) {
    //printInformation(val);
    
    if (canAddNodeToParent(node, parentNode)) {
        moveTreeIteratorToNode(node);
        mafTree<QObject *>::iterator temp_iterator = m_Tree->parent(m_TreeIterator);

        //Remove subtree from m_Tree.
        mafTree<QObject *> cutTree;
        cutTree = m_Tree->cut(m_TreeIterator);
        m_Tree->erase(m_TreeIterator);

        //Insert subTree in new position.
        moveTreeIteratorToNode(parentNode);
        m_Tree->insert(m_TreeIterator, 0, cutTree);

        m_TreeIterator = temp_iterator;
    }
    
    // Needed to update the model connected to the hierarchy.
    Q_EMIT itemReparent(node, parentNode);
}

bool mafHierarchy::canAddNodeToParent(QObject *node, QObject *parent) {
    bool result(false);
    moveTreeIteratorToNode(node);
    mafTree<QObject *>::iterator temp_iterator = m_Tree->parent(m_TreeIterator);
    moveTreeIteratorToNode(parent);
    result = temp_iterator != m_TreeIterator;
    return result;
}

void mafHierarchy::moveTreeIteratorToParent() {
    m_TreeIterator = m_Tree->parent(m_TreeIterator);
}

bool mafHierarchy::hasParent() {
    return m_Tree->root().node() != m_TreeIterator.node();
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
            mafObjectBase *obj = qobject_cast<mafObjectBase *>(n->m_data);
            if (obj) {
                obj->release();
            } else {
                delete n->m_data;
            }
            n->m_data = NULL;
        }
    }
    m_TreeIterator = m_Tree->root();
    m_Tree->erase(m_TreeIterator);
    m_TreeIterator.null();
    
    Q_EMIT clearTree();
}

void mafHierarchy::setIterator(mafTree<QObject *>::iterator iterator) {
    m_TreeIterator = iterator;
}

mafTree<QObject *>::iterator mafHierarchy::iterator() {
    return m_TreeIterator;

}

void mafHierarchy::printInformation(QString &string) {
    for (mafTree<QObject *>::prefix_iterator i = m_Tree->prefix_begin(); i != m_Tree->prefix_end(); ++i) {
        mafTreeNode<QObject *> *n = i.simplify().node();
        QObject *obj = n->m_data;
        if(obj != NULL) {
            string.append(obj->objectName());
            string.append("\n");
        }
    }
}

mafMemento *mafHierarchy::createMemento() const {
    mafMementoHierarchy *mementoHierarchy = new mafMementoHierarchy(this, mafCodeLocation);
    return mementoHierarchy;
}

void mafHierarchy::setMemento(mafMemento *memento, bool deep_memento) {
    //@@ TODO this is a fake hierarchy! because the recursion is shared between all mafObjects!!!!!!!!
    
    this->moveTreeIteratorToRootNode();

    // Design by contract condition.
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    int n = 0;
    int childrenNum = memento->children().size();
    for (n; n < childrenNum; n++) {
        mafMemento *m = (mafMemento *)memento->children().at(n);
        if (m->serializationPattern() == mafSerializationPatternInheritance) {
            //set the memento of the superclass
            Superclass::setMemento(m, deep_memento);
        } else {
            //set the memento of the children memento
            QString objClassType = m->objectClassType();
            mafCore::mafObjectBase *objBase = mafNEWFromString(objClassType);
            mafCore::mafObject *obj = qobject_cast<mafCore::mafObject *>(objBase);
            obj->setMemento(m, deep_memento);
            mafDEL(objBase);
        }
    }
}


