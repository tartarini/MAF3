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

mafMementoDataSet::mafMementoDataSet(const mafObject *obj, mafPoseMatrix *matrix, mafContainerInterface *dataValue, bool binary, const QString code_location)  : mafMemento(obj, code_location) {
  m_DataSizeExported = 0;
  m_DataSizeImported = 0;
  m_TimeStamp = 0;
  
  m_DataSet = const_cast<mafResources::mafDataSet*>(qobject_cast<const mafResources::mafDataSet*>(obj));
  mafMementoPropertyList *list = mementoPropertyList();

  if(matrix) {
      QVariantList matrixList; //should I use double instead of QVariant?

      int r = 0;
      for ( ; r < 4; ++r) {
          int c = 0;
          for ( ; c < 4 ; ++c ) {
              matrixList.append(matrix->get(r, c));
          }
      }

      mafMementoPropertyItem item;
      item.m_Multiplicity = 1;
      item.m_Name = "poseMatrix";
      item.m_Value = matrixList;
      list->append(item);
  }
  if(dataValue) {
      mafMementoPropertyItem item;
      QString encodeType = dataValue->externalCodecType();

      //Store encode type
      item.m_Multiplicity = 1;
      item.m_Name = "encodeType";
      item.m_Value = encodeType;
      list->append(item);

      //Store dataType
      item.m_Multiplicity = 1;
      item.m_Name = "dataType";
      item.m_Value = dataValue->externalDataType();
      list->append(item);

      //Store timeStamp
      item.m_Multiplicity = 1;
      item.m_Name = "timeStamp";
      int timeStamp = obj->property("timeStamp").toDouble();
      item.m_Value = QVariant(timeStamp);
      list->append(item);
                    
      //Store dataHash
      /*item.m_Multiplicity = 1;
      item.m_Name = "dataHash";
      QString hash = obj->objectHash();
      item.m_Value = hash;
      list->append(item);*/

      //Store fileName
      item.m_Multiplicity = 1;
      item.m_Name = "fileName";
      QString fileName(obj->objectHash());
      fileName.append(".vtk");
      item.m_Value = fileName;
      list->append(item);

      //mafDEL(codec);
   }
}

mafMementoDataSet::~mafMementoDataSet() {
}

void mafMementoDataSet::encodeItem(QXmlStreamWriter *XMLStream, QDataStream *binaryStream, QTextStream *textStream, mafMementoPropertyItem item) {
  double dataTime = 0;
  QString dataHash;
   
if(binaryStream != NULL) {
    if (item.m_Name == "encodeType" ) {
      m_EncodeType = item.m_Value.toString();
      *binaryStream << item.m_Value.toString();
    } else if (item.m_Name == "timeStamp" ) {
      *binaryStream << item.m_Value.toDouble();
    } else if (item.m_Name == "fileName") {
      //Generate file name and save external data
      QString path = ((QFile *) binaryStream->device())->fileName().section('/', 0, -2);
      QString fileName(item.m_Value.toString());
      QString url;
      QTextStream(&url) << path << "/" << fileName;
      mafContainerInterface *container = m_DataSet->dataValue();

      mafEventArgumentsList argList;
      argList.append(mafEventArgument(mafCore::mafContainerInterface *, container));
      argList.append(mafEventArgument(QString, url));
      argList.append(mafEventArgument(QString, m_EncodeType));
      mafEventBusManager::instance()->notifyEvent("maf.local.serialization.export", mafEventTypeLocal, &argList);
      *binaryStream << fileName;
    }
  } else if (textStream != NULL) {
    if (item.m_Name == "encodeType" ) {
      m_EncodeType = item.m_Value.toString();
      *textStream << item.m_Value.toString() << endl;;
    } else if (item.m_Name == "timeStamp" ) {
      *textStream << item.m_Value.toDouble();
    } else if (item.m_Name == "fileName") {
      //Generate file name and save external data
      QString path = ((QFile *) textStream->device())->fileName().section('/', 0, -2);
      QString fileName(item.m_Value.toString());
      QString url;
      QTextStream(&url) << path << "/" << fileName;
      mafContainerInterface *container = m_DataSet->dataValue();

      mafEventArgumentsList argList;
      argList.append(mafEventArgument(mafCore::mafContainerInterface *, container));
      argList.append(mafEventArgument(QString, url));
      argList.append(mafEventArgument(QString, m_EncodeType));
      mafEventBusManager::instance()->notifyEvent("maf.local.serialization.export", mafEventTypeLocal, &argList);
      *textStream << fileName << endl;
    } 
  } else if (XMLStream != NULL) {
      if (item.m_Name == "encodeType" ) {
          m_EncodeType = item.m_Value.toString();
      }

      if (item.m_Name == "fileName") {
          //Generate file name and save external data
          QString path = ((QFile *) XMLStream->device())->fileName().section('/', 0, -2);
          QString fileName(item.m_Value.toString());
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
}

QVariant mafMementoDataSet::decodeItem(QXmlStreamWriter *XMLStream, QDataStream *binaryStream, QTextStream *textStream) {
  double timeStamp = 0;
  double dataTime = 0;
  QString dataHash;
  QString encodeType;
  mafMementoPropertyItem item;
  
  if(binaryStream != NULL) {
    *binaryStream >> item.m_Name;
    *binaryStream >> item.m_Multiplicity;
    if (item.m_Name == "fileName") {
      //check if eChild is a file Name
      QString fileName;
      *binaryStream >> fileName;
      QString path = ((QFile *)binaryStream->device())->fileName().section('/', 0, -2);
      QByteArray url;
      url.append(path);
      url.append("/");
      url.append(fileName);
      QUrl u = QUrl::fromEncoded(url);
      if (u.isValid()) {
        //write external file url
        item.m_Value = u.toString();
      } 
    } else if (item.m_Name == "timeStamp" ) {
      *binaryStream >> timeStamp;
      this->setTimeStamp(timeStamp);
      item.m_Value = timeStamp;
    } else if (item.m_Name == "encodeType" ) {
      *binaryStream >> encodeType;
      item.m_Value = encodeType;
    } 
  } else if (textStream != NULL) {
    *textStream >> item.m_Name;
    *textStream >> item.m_Multiplicity;
    if (item.m_Name == "fileName") {
      //check if eChild is a file Name
      QString fileName;
      *textStream >> fileName;
      QString path = ((QFile *)textStream->device())->fileName().section('/', 0, -2);
      QByteArray url;
      url.append(path);
      url.append("/");
      url.append(fileName);
      QUrl u = QUrl::fromEncoded(url);
      if (u.isValid()) {
        //write external file url
        item.m_Value = u.toString();
      }
    } else if (item.m_Name == "timeStamp" ) {
      *textStream >> timeStamp;
      this->setTimeStamp(timeStamp);
      item.m_Value = timeStamp;
    } else if (item.m_Name == "encodeType" ) {
      *textStream >> encodeType;
      item.m_Value = encodeType;
    }
  } else if (XMLStream != NULL) {

  }
  return item.m_Value;
 }
 

