/*
 *  mafMementoResource.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoResource.h"
#include "mafResource.h"
#include "mafObject.h"

using namespace mafCore;
using namespace mafResources;

mafMementoResource::mafMementoResource(const QString code_location)  : mafMemento(code_location) {
}

mafMementoResource::mafMementoResource(const mafObject *obj, const mafResourceList *inputList, const QString code_location) : mafMemento(obj, code_location) {
    REQUIRE(obj != NULL);
    REQUIRE(inputList != NULL);

    QStringList hashLists;
    mafResource *o = NULL;
    Q_FOREACH(o, *inputList) {
        QString hash = o->objectHash();
        hashLists.append(hash);
    }

    mafMementoPropertyList *list = mementoPropertyList();
    mafMementoPropertyItem item;
    item.m_Multiplicity = hashLists.count();
    item.m_Name = "InputList";
    item.m_Value = QVariant(hashLists);
    list->append(item);
}

mafMementoResource::~mafMementoResource() {
}
