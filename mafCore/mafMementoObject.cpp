/*
 *  mafMementoObject.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafMementoObject.h"
#include "mafObject.h"


using namespace mafCore;

mafMementoObject::mafMementoObject(const mafString code_location) : mafMemento(code_location) {
}

mafMementoObject::mafMementoObject(const mafObject *obj, const mafTagList *tagList, mafDictionary *dic, const mafString code_location) : mafMemento(obj, code_location) {
    mafMementoPropertyList *list = mementoPropertyList();
    int i = 0;
    const QMetaObject *meta = obj->metaObject();
    int num = meta->propertyCount();
    for ( ; i < num; ++i) {
        mafMementoPropertyItem item;
        const QMetaProperty qmp = meta->property(i);
        mafString propName = qmp.name();

        mafVariant value = obj->property(propName.toAscii());
        item.m_Multiplicity = 1;
        item.m_Name = qmp.name();
        item.m_Value = value;
        list->append(item);
    }

    mafMementoPropertyItem itemTagList;
    itemTagList.m_Multiplicity = tagList->count();
    itemTagList.m_Name = "TagList";
    itemTagList.m_Value = *tagList;
    list->append(itemTagList);

    mafMementoPropertyItem itemDic;
    itemDic.m_Multiplicity = dic->count();
    itemDic.m_Name = "dictionary";
    itemDic.m_Value = *(dic);
    list->append(itemDic);
}

mafMementoObject::~mafMementoObject() {
}
