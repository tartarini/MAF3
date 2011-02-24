/*
 *  mafMementoViewManager.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoViewManager.h"
#include "mafResource.h"

using namespace mafCore;
using namespace mafResources;

mafMementoViewManager::mafMementoViewManager(const mafString code_location) : mafMemento(code_location) {
}

mafMementoViewManager::mafMementoViewManager(const mafObjectBase *obj, const mafResourceList *vList, const mafString code_location) : mafMemento(code_location) {
    const QMetaObject* meta = obj->metaObject();
    setObjectClassType(meta->className());

    mafMementoPropertyList *list = mementoPropertyList();
    mafMementoPropertyItem item;

    foreach(mafResource *v, *vList) {
        item.m_Name = "ViewType";
        item.m_Multiplicity = 1;
        item.m_Value = mafVariant(v->metaObject()->className());
        list->append(item);
    }
}

mafMementoViewManager::~mafMementoViewManager() {
}
