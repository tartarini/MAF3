/*
 *  mafMemento.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafMemento.h"
#include "mafObject.h"

using namespace mafCore;

mafMemento::mafMemento(const QString code_location) : mafObjectBase(code_location), m_ObjectClassType("") {
    m_MementoPropertyList.clear();
}

mafMemento::mafMemento(const mafObject *obj, const QString code_location) : mafObjectBase(code_location), m_ObjectClassType(""), m_MementoHierarchyType(MementoHierarchyType::INHERIT_MEMENTO) {
    REQUIRE(obj);
    const QMetaObject* meta = obj->metaObject();
    setObjectClassType(meta->className());

    m_MementoPropertyList.clear();
}

 mafMemento::~mafMemento() {
 }

bool mafMemento::isEqual(const mafObjectBase *obj) const {
    // First check the isEqual for the parent class.
    if(Superclass::isEqual(obj)) {
        const mafMemento *memento = qobject_cast<const mafMemento *>(obj);
        if(memento == NULL) {
            return false;
        }
        int i = 0, size = m_MementoPropertyList.count();
        if(size != memento->m_MementoPropertyList.count()) {
            return false;
        }

        if(m_ObjectClassType != memento->objectClassType()) {
            return false;
        }

        for(;i<size;++i) {
            mafMementoPropertyItem item = m_MementoPropertyList[i];
            mafMementoPropertyItem itemObject = memento->m_MementoPropertyList[i];
            if(item.m_Name != itemObject.m_Name || item.m_Multiplicity != itemObject.m_Multiplicity || item.m_Value != itemObject.m_Value) {
                return false;
            }
        }
    }
    return true;
}

void mafMemento::encodeItem(mafMementoPropertyItem *item, QString path) {
  Q_UNUSED(item);
  Q_UNUSED(path);
}

void mafMemento::decodeItem(mafMementoPropertyItem *item, QString path) {
  Q_UNUSED(item);
  Q_UNUSED(path);
}

