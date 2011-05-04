/*
 *  mafMementoDataSetCollection.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
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

/*void mafMementoDataSetCollection::encodeItem(mafMementoPropertyItem *item, QString path) {
  if (item->m_Name == "encodeType" ) {
    m_EncodeType = item->m_Value.toString();
  } else if (item->m_Name == "fileName") {
    //Generate file name and save external data
    QString fileName(item->m_Value.toString());
    QString url;
    QTextStream(&url) << path << "/" << fileName;
    mafContainerInterface *container = m_DataSet->dataValue();

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafContainerInterface *, container));
    argList.append(mafEventArgument(QString, url));
    argList.append(mafEventArgument(QString, m_EncodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.export", mafEventTypeLocal, &argList);
  }
}

void mafMementoDataSetCollection::decodeItem(mafMementoPropertyItem *item, QString path) {
  if (item->m_Name == "fileName") {
    //check if eChild is a file Name
    QString fileName;
    fileName = item->m_Value.toString();
    QByteArray url;
    url.append(path);
    url.append("/");
    url.append(fileName);
    QUrl u = QUrl::fromEncoded(url);
    if (u.isValid()) {
      //write external file url
      item->m_Value = u.toString();
    } 
  } else if (item->m_Name == "timeStamp" ) {
    this->setTimeStamp(item->m_Value.toDouble());
  } 
}*/
 

