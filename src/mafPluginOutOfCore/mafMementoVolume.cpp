/*
 *  mafMementoVolume.cpp
 *  mafResources
 *
 *  Created by Yubo Tao on 21/10/11.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMementoVolume.h"
#include "mafVolume.h"
#include "mafProxy.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

mafMementoVolume::mafMementoVolume(const QString code_location) : mafMemento(code_location) {

}

mafMementoVolume::mafMementoVolume(const mafObject *obj, bool binary, const QString code_location) : mafMemento(obj, code_location) {
    m_EncodeType = "RAW";   // mafSerialization::mafCodecRawBinary
    m_DataSet = const_cast<mafResources::mafVolume*>(qobject_cast<const mafResources::mafVolume*>(obj));

    QVariantList varList;
    mafMementoPropertyItem item;
    mafMementoPropertyList *list = mementoPropertyList();

    // data type
    item.m_Multiplicity = 1;
    item.m_Name         = "dataType";
    item.m_Value        = m_DataSet->dataType();
    list->append(item);

    // bits stored
    item.m_Multiplicity = 1;
    item.m_Name         = "bitsStored";
    item.m_Value        = m_DataSet->bitsStored();
    list->append(item);

    // big Endian
    item.m_Multiplicity = 1;
    item.m_Name         = "bigEndian";
    item.m_Value        = m_DataSet->bigEndian();
    list->append(item);

    // component number
    item.m_Multiplicity = 1;
    item.m_Name         = "componentNum";
    item.m_Value        = m_DataSet->componentNum();
    list->append(item);

    // dimensions
    int *dimensions = m_DataSet->dimensions();
    varList.clear();
    varList.append(dimensions[0]);
    varList.append(dimensions[1]);
    varList.append(dimensions[2]);
    item.m_Multiplicity = varList.count();
    item.m_Name         = "dimensions";
    item.m_Value        = varList;
    list->append(item);

    // spacing
    float *spacing = m_DataSet->spacing();
    varList.clear();
    varList.append(spacing[0]);
    varList.append(spacing[1]);
    varList.append(spacing[2]);
    item.m_Multiplicity = varList.count();
    item.m_Name         = "spacing";
    item.m_Value        = varList;
    list->append(item);

    // matrix
    mafMatrix *matrix = m_DataSet->poseMatrix();
    varList.clear();
    for(int r = 0; r < 4; ++r) {
        for(int c = 0; c < 4; ++c) {
            varList.append(matrix->element(r, c));
        }
    }
    item.m_Multiplicity = varList.count();
    item.m_Name         = "poseMatrix";
    item.m_Value        = varList;
    list->append(item);

    // level number
    item.m_Multiplicity = 1;
    item.m_Name         = "levels";
    item.m_Value        = m_DataSet->calcMultiresolutionLevel();
    list->append(item);

    // brick size
    if (m_DataSet->bricking()) {
        item.m_Multiplicity = 1;
        item.m_Name         = "brickSize";
        item.m_Value        = m_DataSet->brickSize();
        list->append(item);
    }

    // encode type
    item.m_Multiplicity = 1;
    item.m_Name         = "encodeType";
    item.m_Value        = m_DataSet->codecType();
    list->append(item);

    // file name
    item.m_Multiplicity = 1;
    item.m_Name         = "fileName";
    item.m_Value        = m_DataSet->fileName();
    list->append(item);
}

mafMementoVolume::~mafMementoVolume() {

}

void mafMementoVolume::encodeItem(mafCore::mafMementoPropertyItem *item) {
    if (item->m_Name == "fileName") {   // && m_DataSet->modified()
        //Generate file name and save external data
        QString fileName(item->m_Value.toString());
        if(fileName.isEmpty() || (m_DataSet->dataValue() == 0)) return;
        
        QString path;
        QGenericReturnArgument path_val = mafEventReturnArgument(QString, path);
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.request.workingDirectory", mafEventTypeLocal, NULL, &path_val);

        QString url;
        QTextStream(&url) << path << "/" << fileName;
        m_DataSet->setFileName(url);
        mafProxy<mafVolume> container;
        container = m_DataSet;
        QString encodeType = m_DataSet->codecType();
        
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafProxyInterface *, &container));
        argList.append(mafEventArgument(QString, url));
        argList.append(mafEventArgument(QString, encodeType));
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.export", mafEventTypeLocal, &argList);
        m_DataSet->setModified(false);
        m_DataSet->setFileName(fileName);
    }
}

void mafMementoVolume::decodeItem(mafCore::mafMementoPropertyItem *item) {
    if (item->m_Name == "fileName") {
        //check if eChild is a file Name
        QString fileName;
        fileName = item->m_Value.toString();
        QByteArray url;
        
        QString path;
        QGenericReturnArgument path_val = mafEventReturnArgument(QString, path);
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.request.workingDirectory", mafEventTypeLocal, NULL, &path_val);

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