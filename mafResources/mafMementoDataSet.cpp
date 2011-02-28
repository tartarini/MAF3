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
#include "mafDataPipe.h"
#include "mafDataSetCollection.h"
#include "mafExternalDataCodec.h"

using namespace mafCore;
using namespace mafResources;

mafMementoDataSet::mafMementoDataSet(const QString code_location) : mafMemento(code_location) {
}

mafMementoDataSet::mafMementoDataSet(const mafObject *obj, mafPoseMatrix *matrix, mafContainerInterface *dataValue, bool binary, const QString code_location)  : mafMemento(obj, code_location) {
    mafMementoPropertyList *list = mementoPropertyList();

    if(matrix) {
        QList<QVariant> matrixList; //should I use double instead of QVariant?

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
        QString codecType = dataValue->externalCodecType();
        mafExternalDataCodec *codec = (mafExternalDataCodec *)mafNEWFromString(codecType);

        //Store codec type
        item.m_Multiplicity = 1;
        item.m_Name = "codecType";
        item.m_Value = codecType;
        list->append(item);

        //Store dataType
        item.m_Multiplicity = 1;
        item.m_Name = "dataType";
        item.m_Value = dataValue->externalDataType();
        list->append(item);

        //Encode dataValue
        codec->setExternalData(dataValue);
        char *outputString = codec->encode(binary);
        QByteArray array = QByteArray::fromRawData(outputString, codec->stringSize());

        //Store dataSize
        item.m_Multiplicity = 1;
        item.m_Name = "dataSize";
        int size = codec->stringSize();
        item.m_Value = QVariant(size);
        list->append(item);
                      
        //Store dataHash
        item.m_Multiplicity = 1;
        item.m_Name = "dataHash";
        QString hash = obj->objectHash();
        item.m_Value = hash;
        list->append(item);

        //Store dataValue
        item.m_Multiplicity = 1;
        item.m_Name = "dataValue";
        item.m_Value.setValue(array);
        list->append(item);

        mafDEL(codec);
     }
}

mafMementoDataSet::~mafMementoDataSet() {
}
