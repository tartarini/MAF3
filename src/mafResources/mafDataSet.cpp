/*
 *  mafDataSet.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataSet.h"
#include "mafDataBoundaryAlgorithm.h"
#include "mafExternalDataCodec.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

mafDataSet::mafDataSet(const QString code_location) : mafObject(code_location), m_DataValue(NULL), m_DataBoundary(NULL), m_Matrix(NULL), m_DataBoundaryAlgorithm(NULL), m_DataBoundaryAlgorithmName(), m_ExternalDataType(), m_ExternalCodecType(), m_DataLoaded(false), m_FileName(), m_Bounds(NULL) {
    m_Bounds = new mafResources::mafBounds;
    //QObject::connect(this, SIGNAL(loadDataSignal()), this, SLOT(updateDataValue()), Qt::BlockingQueuedConnection);
}

mafDataSet::~mafDataSet() {
    // External data must be destroyed by the creator if no pointer to destructor function has been given.
    mafDEL(m_DataBoundaryAlgorithm);
    mafDEL(m_Bounds);
    if(m_Matrix != NULL) {
        delete m_Matrix;
        m_Matrix = NULL;
    }
}


mafMatrix4x4 *mafDataSet::poseMatrix() {
    if(m_Matrix == NULL) {
        m_Matrix = new mafMatrix4x4();
        m_Matrix->setToIdentity();
    }
    return m_Matrix;
}

QString mafDataSet::poseMatrixString() {
    if(m_Matrix == NULL) {
        m_Matrix = new mafMatrix4x4();
        m_Matrix->setToIdentity();
    }
    QString matrixString;
    //Create a string with the values of matrix element separated by a blank space.
    int r = 0;
    for ( ; r < 4; ++r) {
        int c = 0;
        for ( ; c < 4 ; ++c ) {
            matrixString.append(QString::number((*m_Matrix)(r,c)));
            if (r != 3 || c != 3) {
                matrixString.append(" ");
            }
        }
    }
    return matrixString;
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
        Q_EMIT dataValueDisconnected();
    }
    m_DataValue = data_value;
    if (m_DataValue) {
        QString dataType = data_value->externalDataType();
        m_ExternalDataType = dataType;
        QString codecType = data_value->externalCodecType();
        m_ExternalCodecType = codecType;
        if(m_DataValue != NULL) {
            Q_EMIT dataValueConnected();
            m_DataLoaded = true;
            updateBounds();
        }
    }
    setModified();
}

mafProxyInterface *mafDataSet::dataValue() {
    if (m_DataValue == NULL) {
//        Q_EMIT loadDataSignal();
        updateDataValue();
    }
    return m_DataValue;
}

void mafDataSet::setPoseMatrix(const mafMatrix4x4 *matrix) {
    if(matrix == NULL) {
        return;
    }
    
    m_Matrix = new mafMatrix4x4(*matrix);
    setModified();
}

void mafDataSet::setPoseMatrixString(const QString matrixString) {
    if(matrixString.isEmpty()) {
        return;
    }
    QStringList list = matrixString.split(" ");
    //Check if string represents a matrix
    int numElement = list.count();
    int i = 0;
    bool ok;
    for ( ; i < numElement; i++ ) {
        list[i].toDouble(&ok);
        if (!ok || numElement != 16) {
            QByteArray ba = mafTr("Trying to assign an invalid string to pose Matrix.").toLatin1();
            qWarning("%s", ba.data());
            return;
        }
    }

    mafMatrix4x4 *matrix = new mafMatrix4x4();
    int counter = 0;
    int r = 0;
    for ( ; r < 4; ++r) {
        int c = 0;
        for ( ; c < 4 ; ++c) {
            double val = list[counter].toDouble();
            (*matrix)(r,c) = val;
            ++counter;
        }
    }
    m_Matrix = new mafMatrix4x4(*matrix);
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
    Q_FOREACH(item, *list) {
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
    mafCore::mafProxyInterface *container = NULL;
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_FileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafProxyInterface *, container);
    qDebug() << "mafDataset" << QThread::currentThread();
            QThread *ctb = this->thread();        
      this->moveToThread(QCoreApplication::instance()->thread());
      QThread *cta = this->thread();
//      QMutex m;
//      m.lock();
//      
      
      mafEventBusManager::instance()->notifyEvent("maf.local.serialization.import", mafEventTypeLocal, &argList, &ret_val);
      
//      mafEventBusManager::instance()->w.wait(&m);
//      m.unlock();
    
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

void mafDataSet::setBounds(mafResources::mafBoundsPointer bounds) {
    if(m_Bounds != bounds) {
        mafDEL(m_Bounds);
        m_Bounds = bounds;
        m_Bounds->retain();
    }
}

void mafDataSet::updateBounds() {
    mafDataBoundaryAlgorithm *boundary = NULL;
    boundary = this->boundaryAlgorithm();
    double b[6] = {-1,1,-1,1,-1,1};
    if(boundary != NULL && m_DataValue != NULL){
        boundary->calculateBoundary(m_DataValue);
        boundary->bounds(b);
    }
    m_Bounds->setBounds(b);
}
