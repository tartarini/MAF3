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
#include "mafHierarchy.h"

using namespace mafCore;

mafMementoHierarchy::mafMementoHierarchy(const QString code_location)  : mafMemento(code_location) {
}

mafMementoHierarchy::mafMementoHierarchy(const mafObject *obj, const QString code_location) : mafMemento(obj, code_location) {
    REQUIRE(obj != NULL);
    
    QStringList hashLists;
    QObject *o = NULL;
    
    const mafHierarchy *tree = qobject_cast<const mafHierarchy*>(obj);
    
    //traverse tree and memorize element hash and parent hash, then put them inside a list.
    
    
    
    mafMementoPropertyList *list = mementoPropertyList();
    mafMementoPropertyItem item;
    item.m_Multiplicity = hashLists.count();
    item.m_Name = "Hierarchy";
    item.m_Value = QVariant(hashLists);
    list->append(item);
}

mafMementoHierarchy::~mafMementoHierarchy() {
}
