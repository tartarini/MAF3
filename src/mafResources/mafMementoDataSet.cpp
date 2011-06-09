/*
 *  mafMementoDataSet.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoDataSet.h"
#include "mafPipeData.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafExternalDataCodec.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafMementoDataSet::mafMementoDataSet(const QString code_location) : mafMemento(code_location) {

}

mafMementoDataSet::mafMementoDataSet(const mafObject *obj, bool binary, const QString code_location)  : mafMemento(obj, code_location) {
  m_DataSizeExported = 0;
  m_DataSizeImported = 0;
  
  m_DataSet = const_cast<mafResources::mafDataSet*>(qobject_cast<const mafResources::mafDataSet*>(obj));
  mafMementoPropertyList *list = mementoPropertyList();
  mafPoseMatrix *matrix = m_DataSet->poseMatrix();

  if(matrix) {
      QVariantList matrixList; //should I use double instead of QVariant?

      int r = 0;
      for ( ; r < 4; ++r) {
          int c = 0;
          for ( ; c < 4 ; ++c ) {
              matrixList.append(cvmGet(matrix,r,c));

          }
      }
      mafMementoPropertyItem item;
      item.m_Multiplicity = matrixList.count();
      item.m_Name = "poseMatrix";
      item.m_Value = matrixList;
      list->append(item);
  }
      mafMementoPropertyItem item;
     
      //Store fileName
      item.m_Multiplicity = 1;
      item.m_Name = "fileName";
      QString fileName(obj->objectHash());
      fileName.append(".");
      fileName.append(m_DataSet->externalCodecType().toLower());
      item.m_Value = fileName;
      list->append(item);
}

mafMementoDataSet::~mafMementoDataSet() {
}

void mafMementoDataSet::encodeItem(mafMementoPropertyItem *item, QString path) {
    if (item->m_Name == "fileName" && m_DataSet->modified()) {
        //Generate file name and save external data
        QString fileName(item->m_Value.toString());
        QString url;
        QTextStream(&url) << path << "/" << fileName;
        mafProxyInterface *container = m_DataSet->dataValue();
        QString encodeType = container->externalCodecType();

        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafProxyInterface *, container));
        argList.append(mafEventArgument(QString, url));
        argList.append(mafEventArgument(QString, encodeType));
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.export", mafEventTypeLocal, &argList);
        m_DataSet->setModified(false);
    }
}

void mafMementoDataSet::decodeItem(mafMementoPropertyItem *item, QString path) {
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
  } 
}
 

