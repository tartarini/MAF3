/*
 *  mafMementoObject.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafMementoObject.h"
#include "mafObject.h"


using namespace mafCore;

mafMementoObject::mafMementoObject(const QString code_location) : mafMemento(code_location) {
}

mafMementoObject::mafMementoObject(const mafObject *obj, const QString code_location) : mafMemento(obj, code_location) {
    mafMementoPropertyList *list = mementoPropertyList();
    int i = 0;
    const QMetaObject *meta = obj->metaObject();
    int num = meta->propertyCount();
    for ( ; i < num; ++i) {
        mafMementoPropertyItem item;
        const QMetaProperty qmp = meta->property(i);
        QString propName = qmp.name();

        QVariant value = obj->property(propName.toAscii());

        if (value.canConvert(QVariant::List)) {
            item.m_Multiplicity = value.toList().count();
        } else if(value.canConvert(QVariant::Map)){
            item.m_Multiplicity = value.toMap().count();
        } else if(value.canConvert(QVariant::Hash)){
            item.m_Multiplicity = value.toHash().count();
        } else {
            item.m_Multiplicity = 1;
        }
        item.m_Name = qmp.name();
        item.m_Value = value;
        list->append(item);
    }

    mafMementoPropertyItem itemTagList;
    itemTagList.m_Multiplicity = obj->tagList()->count();
    itemTagList.m_Name = "TagList";
    itemTagList.m_Value = *obj->tagList();
    list->append(itemTagList);

    mafMementoPropertyItem itemScriptList;
    itemScriptList.m_Multiplicity = obj->scriptList()->count();
    itemScriptList.m_Name = "ScriptList";
    itemScriptList.m_Value = *obj->scriptList();
    list->append(itemScriptList);

    mafMementoPropertyItem itemDic;
    itemDic.m_Multiplicity = obj->dictionary()->count();
    itemDic.m_Name = "dictionary";
    itemDic.m_Value = *(obj->dictionary());
    list->append(itemDic);
}

mafMementoObject::~mafMementoObject() {
}
