/*
 *  mafMementoVME.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoVME.h"
#include "mafMementoDataSetCollection.h"
#include "mafPipeData.h"
#include "mafDataSetCollection.h"
#include "mafDataSet.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafMementoVME::mafMementoVME(const QString code_location) : mafMemento(code_location) {
}

mafMementoVME::mafMementoVME(const mafObject *obj, bool binary, const QString code_location)  : mafMemento(obj, code_location) {    
    mafVME *vme = qobject_cast<mafResources::mafVME*>((mafObject *)obj);
    REQUIRE(vme);

    mafMementoPropertyList *list = mementoPropertyList();
    
    if (vme->dataSetCollection()) {
      mafDataSetCollection *collection = vme->dataSetCollection();
      mafMementoDataSetCollection *mementoDataSetCollection = (mafMementoDataSetCollection *)collection->createMemento();
      //Set serializationPattern of the memento created as "Composition" because is not an ancestor of this memento
      mementoDataSetCollection->setSerializationPattern(mafSerializationPatternComposition);
      mementoDataSetCollection->setParent(this);
      //This memento IS NOT an Inherit memento.
    }

    mafPipeData *pipe = vme->dataPipe();
    if(pipe) {
        mafMementoPropertyItem item;
        item.m_Multiplicity = 1;
        item.m_Name = "mafPipeData";
        item.m_Value = QVariant(pipe->metaObject()->className());
        list->append(item);

        int i = 0;
        int num = obj->metaObject()->propertyCount();
        for ( ; i < num; ++i) {
            mafMementoPropertyItem item;
            const QMetaProperty qmp = obj->metaObject()->property(i);
            QString propName = qmp.name();
            QVariant value = obj->property(propName.toAscii());
            item.m_Multiplicity = 1;
            item.m_Name = qmp.name();
            item.m_Value = value;
            list->append(item);
        }
     }

    QVariantList boundsList = vme->bounds();
    if(!boundsList.empty()){
        mafMementoPropertyItem item;
        item.m_Multiplicity = boundsList.size();
        item.m_Name = "vmeBounds";
        item.m_Value = QVariant(boundsList);
        list->append(item);
    }
}

mafMementoVME::~mafMementoVME() {
}
