/*
 *  mafMementoDataSetCollection.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/05/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoDataSetCollection.h"
#include "mafPipeData.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafExternalDataCodec.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafMementoDataSetCollection::mafMementoDataSetCollection(const QString code_location) : mafMemento(code_location) {

}

mafMementoDataSetCollection::mafMementoDataSetCollection(const mafObject *obj, const QString code_location)  : mafMemento(obj, code_location) {
  m_DataSetCollection = const_cast<mafResources::mafDataSetCollection*>(qobject_cast<const mafResources::mafDataSetCollection*>(obj));
  mafMementoPropertyList *list = mementoPropertyList();
  const mafDataSetMap *map = m_DataSetCollection->collectionMap();
  mafMementoPropertyItem item;

  mafDataSetMap::const_iterator iter = map->constBegin();
  while(iter != map->constEnd()) {
    item.m_Name = "timeStamp";
    item.m_Multiplicity = 1;
    item.m_Value = iter.key();
    list->append(item);

    mafDataSet *dataSet = iter.value();

    //call mafMementoDataSet
    mafMementoDataSet *mementoDataSet = (mafMementoDataSet*)dataSet->createMemento();
   //Set serializationPattern of the memento created as "Composition" because is not an ancestor of this memento
    mementoDataSet->setSerializationPattern(mafSerializationPatternComposition);
    mementoDataSet->setParent(this);
    ++iter;
  }
}

mafMementoDataSetCollection::~mafMementoDataSetCollection() {
}

 

