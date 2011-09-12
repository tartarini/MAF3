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
  mafMatrix *matrix = m_DataSet->poseMatrix();

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

void mafMementoDataSet::encodeItem(mafMementoPropertyItem *item) {
    bool ignoreModified(false);
    QGenericReturnArgument ret_val = mafEventReturnArgument(bool, ignoreModified);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.ignoreModified", mafEventTypeLocal, NULL, &ret_val);

    bool checkModified = ignoreModified || m_DataSet->modified();
    if (item->m_Name == "fileName" && checkModified) {
        //Generate file name and save external data
        QString fileName(item->m_Value.toString());
        QString url;
        
        QString path;
        QGenericReturnArgument path_val = mafEventReturnArgument(QString, path);
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.request.workingDirectory", mafEventTypeLocal, NULL, &path_val);

        QDir p(path);
        if(!p.exists()) {
            qWarning() << "Invalid Path: data will be saved in the application directory.";
            path = QDir::toNativeSeparators("./");
        }
        
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

void mafMementoDataSet::decodeItem(mafMementoPropertyItem *item) {
    if (item->m_Name == "fileName") {
        //check if eChild is a file Name
        QString fileName;
        fileName = item->m_Value.toString();
          
        QString path;
        QGenericReturnArgument path_val = mafEventReturnArgument(QString, path);
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.request.workingDirectory", mafEventTypeLocal, NULL, &path_val);
        
        QDir p(path);
        if(!p.exists()) {
            qWarning() << "Invalid Path: data will be saved in the application directory.";
            path = QDir::toNativeSeparators("./");
        }
          
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
 

