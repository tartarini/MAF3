/*
 *  mafMementoHierarchy.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoHierarchy.h"

using namespace mafCore;

mafMementoHierarchy::mafMementoHierarchy(const QString code_location)  : mafMemento(code_location) {
}

mafMementoHierarchy::mafMementoHierarchy(const mafObject *obj, const QString code_location) : mafMemento(obj, code_location) {
    REQUIRE(obj != NULL);
    m_Hierarchy = qobject_cast<mafCore::mafHierarchyPointer>((mafObject *)obj);
    
    QStringList hashLists;
    QObject *o = NULL;
    
    const mafHierarchy *tree = qobject_cast<const mafHierarchy*>(obj);
    
    //traverse tree and memorize element hash and parent hash, then put them inside a list.
    m_Hierarchy->moveTreeIteratorToRootNode();
    traverseTree();
    
    mafMementoPropertyList *list = mementoPropertyList();
    mafMementoPropertyItem item;
    item.m_Multiplicity = hashLists.count();
    item.m_Name = "Hierarchy";
    item.m_Value = QVariant(hashLists);
    list->append(item);
}

mafMementoHierarchy::~mafMementoHierarchy() {
}

void mafMementoHierarchy::traverseTree() {
    QObject *data = m_Hierarchy->currentData();

    int i = 0, size = m_Hierarchy->currentNumberOfChildren();
    for(;i < size; ++i) {
        m_Hierarchy->moveTreeIteratorToNthChild(i);
        QObject *obj = m_Hierarchy->currentData();
        
        mafObject* serializableObj = qobject_cast<mafObject*>(obj);
        mafMemento *memento = serializableObj->createMemento();
        
        memento->setSerializationPattern(mafSerializationPatternComposition);
        memento->setParent(this);
        
        traverseTree();
        m_Hierarchy->moveTreeIteratorToParent();
    }
}
