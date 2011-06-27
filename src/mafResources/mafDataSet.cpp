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

mafDataSet::mafDataSet(const QString code_location) : mafObject(code_location), m_DataValue(NULL), m_DataBoundary(NULL), m_Matrix(NULL), m_DataBoundaryAlgorithm(NULL), m_DataBoundaryAlgorithmName(), m_ExternalDataType(), m_ExternalCodecType(), m_DataLoaded(false), m_FileName() {
    for(int i = 0; i < 6; i++) {
        m_Bounds.append(0);
    }
}

mafDataSet::~mafDataSet() {
    // External data must be destroyed by the creator if no pointer to destructor function has been given.
    mafDEL(m_DataBoundaryAlgorithm);
    if(m_Matrix != NULL) {
        delete m_Matrix;
        m_Matrix = NULL;
    }
}


mafMatrix *mafDataSet::poseMatrix() {
    if(m_Matrix == NULL) {
        m_Matrix = new mafMatrix();
        m_Matrix->setIdentity();
    }
    return m_Matrix;
}

void mafDataSet::setBoundaryAlgorithm(mafDataBoundaryAlgorithm *algorithm) {
    if(m_DataBoundaryAlgorithm != algorithm) {
        mafDEL(m_DataBoundaryAlgorithm);
        m_DataBoundaryAlgorithm = algorithm;
        m_DataBoundaryAlgorithmName = algorithm->metaObject()->className();
    }
}

void mafDataSet::setBoundaryAlgorithmName(QString dataBoundaryAlgorithmName){
    if(m_DataBoundaryAlgorithmName != dataBoundaryAlgorithmName) {
        m_DataBoundaryAlgorithmName = dataBoundaryAlgorithmName;
        mafDEL(m_DataBoundaryAlgorithm);
        m_DataBoundaryAlgorithm = (mafDataBoundaryAlgorithm *)mafNEWFromString(dataBoundaryAlgorithmName);
        updateBounds();
    }
}

mafCore::mafProxyInterface *mafDataSet::dataBoundary() {
    if(m_DataBoundaryAlgorithm != NULL) {
        m_DataBoundary = m_DataBoundaryAlgorithm->calculateBoundary(m_DataValue, m_Matrix);
    }
    return m_DataBoundary;
}

void mafDataSet::setDataValue(mafProxyInterface *data_value) {
    if(m_DataValue == data_value)
        return; // Data is equal to that one already present.

    if(m_DataValue != NULL) {
        emit(dataValueDisconnected());
    }
    m_DataValue = data_value;
    if (m_DataValue) {
        QString dataType = data_value->externalDataType();
        m_ExternalDataType = dataType;
        QString codecType = data_value->externalCodecType();
        m_ExternalCodecType = codecType;
        if(m_DataValue != NULL) {
            emit(dataValueConnected());
            m_DataLoaded = true;
            updateBounds();
        }
    }
    setModified();
}

mafProxyInterface *mafDataSet::dataValue() {
  if (m_DataValue == NULL) {
    updateDataValue();
  }
  return m_DataValue;
}

void mafDataSet::setPoseMatrix(const mafMatrix *matrix) {
    if(matrix == NULL) {
        return;
    }
    
    m_Matrix = matrix->clone();
    setModified();
}

mafMemento *mafDataSet::createMemento() const {
    mafMemento *m = Superclass::createMemento();
    mafMementoDataSet *mementoDataSet = new mafMementoDataSet(this, mafCodeLocation);
    m->setParent(mementoDataSet);

    return mementoDataSet;
}

void mafDataSet::setMemento(mafMemento *memento, bool deep_memento) {
    // Design by contract condition.
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    int n = 0;
    int childrenNum = memento->children().size();
    for (n; n < childrenNum; n++) {
        mafMemento *m = (mafMemento *)memento->children().at(n);
        if (m->serializationPattern() == mafSerializationPatternInheritance) {
            //set the memento of the superclass
            Superclass::setMemento(m, true);
          } else {
            //set the memento of the children memento
            QString objClassType = m->objectClassType();
            mafCore::mafObjectBase *objBase = mafNEWFromString(objClassType);
            mafCore::mafObject *obj = qobject_cast<mafCore::mafObject *>(objBase);
            obj->setMemento(m, deep_memento);
            mafDEL(objBase);
          }
    }

    QString encodeType;
    double timeStamp = 0;

    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    foreach(item, *list) {
        if(item.m_Name == "poseMatrix") {
            //Restore the pose matrix
            mafMatrix *mat = new mafMatrix();
            int counter = 0;
            int r = 0;
            for ( ; r < 4; ++r) {
                int c = 0;
                for ( ; c < 4 ; ++c) {
                    double val = item.m_Value.toList()[counter].toDouble();
                    mat->setElement(r,c,val);
                    ++counter;
                }
            }
            this->setPoseMatrix(mat);
        } 
        if (item.m_Name == "fileName") {
            //Save informations about external file, and load data later, when the data is needed.
            QString nameOfFile = item.m_Value.toString();
            m_FileName = nameOfFile;
        }
    }
    setModified(false);
}

void mafDataSet::updateDataValue() {
  if (!m_FileName.isEmpty()){
    QString encodeType = externalCodecType();
    mafCore::mafProxyInterface *container;
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_FileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafProxyInterface *, container);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.import", mafEventTypeLocal, &argList, &ret_val);
    if (container != NULL) {
        setDataValue(container);
        setExternalCodecType(encodeType);
        m_DataValue->setExternalCodecType(encodeType);
        setModified(false);
    } else {
        QString err_msg(mafTr("Unable to load data form file '%1'").arg(m_FileName));
        qCritical() << err_msg;
    }
  }
}

void mafDataSet::setBounds(QVariantList bounds) {
    m_Bounds.clear();
    m_Bounds.append(bounds);
    setModified();
}

void mafDataSet::updateBounds() {
    mafDataBoundaryAlgorithm *boundary = NULL;
    boundary = this->boundaryAlgorithm();
    if(boundary != NULL){
        boundary->calculateBoundary(m_DataValue);
        double b[6];
        boundary->bounds(b);
        int i = 0;
        for(; i < 6; ++i) {
            m_Bounds[i] = b[i];
        }
    } else {
        int i = 0;
        for(; i < 6; ++i) {
            m_Bounds[i] = (i%2 == 0) ? 1 : -1;
        }
    }
}
