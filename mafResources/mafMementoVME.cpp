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
#include "mafMementoDataSet.h"
#include "mafDataPipe.h"
#include "mafDataSetCollection.h"
#include "mafDataSet.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafMementoVME::mafMementoVME(const mafString code_location) : mafMemento(code_location) {
}

mafMementoVME::mafMementoVME(const mafObject *obj, bool binary, const mafString code_location)  : mafMemento(obj, code_location) {
    mafVME *vme = qobject_cast<mafResources::mafVME*>((mafObject *)obj);
    REQUIRE(vme);

    mafMementoPropertyList *list = mementoPropertyList();
    mafDataSetCollection *collection = vme->dataSetCollection();
    if(collection) {
        const mafDataSetMap *map = collection->collectionMap();
        mafMementoPropertyItem item;

        mafDataSetMap::const_iterator iter = map->constBegin();
        while(iter != map->constEnd()) {
            item.m_Name = "mafDataSetTime";
            item.m_Multiplicity = 1;
            item.m_Value = iter.key();
            list->append(item);
            mafDataSet *dataSet = iter.value();
            //call mafMementoDataSet
            mafMementoPropertyItem dataSetItem;
            mafMementoDataSet *mementoDataSet = new mafMementoDataSet(dataSet, dataSet->poseMatrix(), dataSet->dataValue(), binary, mafCodeLocation);
            mafMementoPropertyList *dataSetList = mementoDataSet->mementoPropertyList();
            foreach(dataSetItem, *dataSetList) {
                list->append(dataSetItem);
            }
            mafDEL(mementoDataSet);
            ++iter;
        }
    }

    mafDataPipe *pipe = vme->dataPipe();
    if(pipe) {
        mafMementoPropertyItem item;
        item.m_Multiplicity = 1;
        item.m_Name = "mafDataPipe";
        item.m_Value = mafVariant(pipe->metaObject()->className());
        list->append(item);

        int i = 0;
        int num = obj->metaObject()->propertyCount();
        for ( ; i < num; ++i) {
            mafMementoPropertyItem item;
            const QMetaProperty qmp = obj->metaObject()->property(i);
            mafString propName = qmp.name();
            mafVariant value = obj->property(propName.toAscii());
            item.m_Multiplicity = 1;
            item.m_Name = qmp.name();
            item.m_Value = value;
            list->append(item);
        }
     }

    QVariantList boundsList = vme->bounds();
    if(!boundsList.empty()){
        mafMementoPropertyItem item;
        item.m_Multiplicity = 6;
        item.m_Name = "vmeBounds";
        item.m_Value = mafVariant(boundsList);
        list->append(item);
    }
}

mafMementoVME::~mafMementoVME() {
}
