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

mafDataSet::mafDataSet(const QString code_location) : mafObject(code_location), m_DataValue(NULL), m_DataBoundary(NULL), m_Matrix(NULL), m_DataBoundaryAlgorithm(NULL), m_TimeStamp(0) {
}

mafDataSet::~mafDataSet() {
    // External data must be destroyed by the creator if no pointer to destructor function has been given.
    mafDEL(m_DataBoundaryAlgorithm);
    delete m_Matrix;
    m_Matrix = NULL;
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

mafContainerInterface *mafDataSet::dataValue() {
  if (m_DataValue == NULL)
  {
    updateDataValue();
  }
  return m_DataValue;
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
    return new mafMementoDataSet(this, mafCodeLocation);
}

void mafDataSet::setMemento(mafMemento *memento, bool binary, bool deep_memento) {
    Q_UNUSED(deep_memento);

    // Design by contract condition.
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    int n = 0;
    int childrenNum = memento->children().size();
    for (n; n < childrenNum; n++) {
      mafMemento *m = (mafMemento *)memento->children().at(n);
      MementoHierarchyType type = m->mementoHierarchyType();
      if (type == INHERIT_MEMENTO) {
        //set the memento of the superclass
        Superclass::setMemento(m, deep_memento);
      } else {
        //set the memento of the children memento
        QString objClassType = m->objectClassType();
        mafCore::mafObjectBase *objBase = mafNEWFromString(objClassType);
        mafCore::mafObject *obj = qobject_cast<mafCore::mafObject *>(objBase);
        obj->setMemento(m, deep_memento);
      }
    }

    QString encodeType;
    double timeStamp = 0;

    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    foreach(item, *list) {
        if(item.m_Name == "poseMatrix") {
            //Restore the pose matrix
            mafPoseMatrix *mat = new mafPoseMatrix();
            int counter = 0;
            int r = 0;
            for ( ; r < 4; ++r) {
                int c = 0;
                for ( ; c < 4 ; ++c) {
                    double val = item.m_Value.toList()[counter].toDouble();
                    mat->put(r,c,val);
                    ++counter;
                }
            }
            this->setPoseMatrix(mat);
        } else if (item.m_Name == "encodeType") {
            //Restore codec type
            encodeType = item.m_Value.toString();
        } else if (item.m_Name == "timeStamp") {
            //Restore time stamp
            this->setTimeStamp(item.m_Value.toDouble());
        } else if (item.m_Name == "fileName") {
            //Read from external file
            //Save informations, and load data later...
            QString nameOfFile = item.m_Value.toString();
            m_DataFileInfo.fileName = nameOfFile;
            m_DataFileInfo.encodeType = encodeType;
         }
    }
}

void mafDataSet::updateDataValue() {
  if (!m_DataFileInfo.fileName.isEmpty() && !m_DataFileInfo.encodeType.isEmpty()){
    mafCore::mafContainerInterface *container;
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_DataFileInfo.fileName));
    argList.append(mafEventArgument(QString, m_DataFileInfo.encodeType));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafContainerInterface *, container);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.import", mafEventTypeLocal, &argList, &ret_val);
    m_DataValue = container;
  } else {
    qDebug() << mafTr("Unable to load data set!");
  }
}
