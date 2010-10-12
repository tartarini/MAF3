/*
 *  mafDataSet.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataSet.h"
#include "mafDataBoundaryAlgorithm.h"
#include "mafExternalDataCodec.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

mafDataSet::mafDataSet(const mafString code_location) : mafObject(code_location), m_DataValue(NULL), m_DataBoundary(NULL), m_Matrix(NULL), m_DataBoundaryAlgorithm(NULL) {
}

mafDataSet::~mafDataSet() {
    // External data must be destroyed by the creator if no pointer to destructor function has been given.
    mafDEL(m_DataBoundaryAlgorithm);
    mafDEL(m_Matrix);
}

void mafDataSet::setBoundaryAlgorithm(mafDataBoundaryAlgorithm *algorithm) {
    if(m_DataBoundaryAlgorithm != algorithm) {
        mafDEL(m_DataBoundaryAlgorithm);
        m_DataBoundaryAlgorithm = algorithm;
    }
}

mafCore::mafContainerInterface *mafDataSet::dataBoundary() {
    if(m_DataBoundaryAlgorithm != NULL) {
        m_DataBoundary = m_DataBoundaryAlgorithm->calculateBoundary(m_DataValue, m_Matrix);
    }
    return m_DataBoundary;
}

void mafDataSet::setDataValue(mafContainerInterface *data_value) {
    if(m_DataValue == data_value)
        return; // Data is equal to that one already present.

    if(m_DataValue != NULL) {
        emit(dataValueDisconnected());
    }

    m_DataValue = data_value;
    if(m_DataValue != NULL) {
        emit(dataValueConnected());
    }
}

void mafDataSet::setPoseMatrix(const mafPoseMatrix *matrix) {
    if(matrix == NULL) {
        return;
    }

    if(m_Matrix == NULL) {
        m_Matrix = new mafPoseMatrix();
    }

    *m_Matrix = *matrix;
}

mafMemento *mafDataSet::createMemento() const {
    return new mafMementoDataSet(this, m_Matrix, m_DataValue, mafCodeLocation);
}

void mafDataSet::setMemento(mafMemento *memento, bool deep_memento) {
    Q_UNUSED(deep_memento);

    // Design by contract condition.
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());
    mafString codecType;
    mafString dataType;
    int dataSize = 0;

    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    foreach(item, *list) {
        if(item.m_Name == "poseMatrix") {
            //Create the pose matrix
            mafPoseMatrix *mat = new mafPoseMatrix();
            int counter = 0;
            int r = 0;
            for ( ; r < 4; r++) {
                int c = 0;
                for ( ; c < 4 ; c++) {
                    double val = item.m_Value.toList()[counter].toDouble();
                    mat->put(r,c,val);
                    counter++;
                }
            }
            this->setPoseMatrix(mat);
        } else if (item.m_Name == "codecType") {
            //Restore codec type
            codecType = item.m_Value.toString();
        } else if (item.m_Name == "dataType") {
            //Restore data type
            dataType = item.m_Value.toString();
        } else if (item.m_Name == "dataSize") {
            //Restore data size
            dataSize = item.m_Value.toInt();
        } else if (item.m_Name == "dataValue") {
            //Read from external file
            char *value;
            mafByteArray stringArray;
            mafEventArgumentsList argList;
            mafUrl url = QUrl::fromEncoded(item.m_Value.toByteArray());
            if (url.isValid()) {
                mafString urlString = url.toString();
                argList.append(mafEventArgument(mafString, urlString));
                mafEventBusManager::instance()->notifyEvent("maf.local.serialization.loadExternalData", mafEventTypeLocal, &argList, &mafEventReturnArgument(mafByteArray,stringArray));
                value = (char*)stringArray.constData();
            } else {
                value = (char*)item.m_Value.toByteArray().constData();
            }
            //Decode dataSet
            mafExternalDataCodec *codec = (mafExternalDataCodec *)mafNEWFromString(codecType);

            codec->setStringSize(dataSize);
            codec->decode(value);

            this->setDataValue(codec->externalData());
            m_DataValue->setExternalCodecType(codecType);
            m_DataValue->setExternalDataType(dataType);
            mafDEL(codec);
        }
    }
}
