/*
 *  mafVME.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVME.h"
#include "mafPipeData.h"
#include "mafDataSet.h"
#include "mafMementoDataSet.h"
#include "mafInteractor.h"
#include "mafDataBoundaryAlgorithm.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafVME::mafVME(const QString code_location) : mafResource(code_location), m_Interactor(NULL), m_DataSetCollection(NULL), m_DataPipe(NULL), m_Binary(true), m_CanRead(true), m_CanWrite(true) {
    m_Lock = new QReadWriteLock(QReadWriteLock::Recursive);
    mafId time_set_id = mafIdProvider::instance()->idValue("TIME_SET");
    if(time_set_id != -1) {
        mafRegisterLocalCallback("TIME_SET", this, "setTimestamp(double)")
    }
    m_MementoDataSetHash.clear();
    connect(this, SIGNAL(modifiedObject()), this, SLOT(execute()));
}

mafVME::~mafVME() {
    mafDEL(m_DataSetCollection);
    mafDEL(m_DataPipe);

    // delete all the created mementos for the dataset collection.
    QHash<mafMementoDataSet *, double>::iterator iter = m_MementoDataSetHash.begin();
    while(iter != m_MementoDataSetHash.end()) {
        mafMementoDataSet *memento = iter.key();
        mafDEL(memento);
        ++iter;
    }
    m_MementoDataSetHash.clear();
    delete m_Lock;
}

void mafVME::setBounds(QVariantList bounds) {
    QWriteLocker locker(m_Lock);
    m_Bounds.clear();
    m_Bounds.append(bounds);
    setModified();
}

void mafVME::setCanRead(bool lock) {
    if ( lock == m_CanRead ) {
        return;
    }
    m_Lock->lockForWrite();
    m_CanRead = lock;
    if ( !m_CanRead ) {
        // VME can not be accessed in read mode => noway also for the write side.
        m_CanWrite = false;
    }
    m_LockStatus = m_CanRead ? mafObjectLockNone : mafObjectLockRead;
    m_Lock->unlock();
    emit vmeLocked(m_LockStatus);
}

void mafVME::setCanWrite(bool lock) {
    if ( lock == m_CanWrite || !m_CanRead ) {
        return;
    }
    m_Lock->lockForWrite();
    m_CanWrite = lock;
    m_LockStatus = m_CanWrite ? mafObjectLockNone : mafObjectLockWrite;
    m_Lock->unlock();
    emit vmeLocked(m_LockStatus);
}

void mafVME::setTimestamp(double t) {
    m_Lock->lockForWrite();
    dataSetCollection()->setTimestamp(t);
    setModified();
    m_Lock->unlock();
    execute();
}

void mafVME::setInteractor(mafInteractor *i) {
    QWriteLocker locker(m_Lock);
    if ( i == m_Interactor ) {
        return;
    }
    mafDEL(m_Interactor);
    m_Interactor = i;
}

void mafVME::setDataPipe(const QString &pipe_type) {
    mafObjectBase *obj = mafNEWFromString(pipe_type);
    mafPipeData *new_pipe = qobject_cast<mafPipeData *>(obj);
    if ( new_pipe ) {
        setDataPipe(new_pipe);
    } else {
        qWarning("%s", mafTr("%1 does not represent a type of mafPipeData.").arg(pipe_type).toAscii().data());
    }
    mafDEL(obj);
}

void mafVME::setDataPipe(mafPipeData *pipe) {
    if ( m_DataPipe == pipe ) {
        return;
    }
    
    if ( pipe ) {
        m_Lock->lockForWrite();
        pipe->setInput(this);
        mafDEL(m_DataPipe);
        m_DataPipe = pipe;
        m_DataPipe->retain();
        setModified();
        m_Lock->unlock();
        execute();
    }
}

void mafVME::execute() {
    if(m_DataPipe) {
        double timeStamp = dataSetCollection()->timeStamp();
        m_DataPipe->output(timeStamp);
    }
}

void mafVME::detatchFromTree() {
    emit(detatched());
}

void mafVME::setSelected(bool sel) {
    if(m_Selected != sel) {
        m_Lock->lockForWrite();
        m_Selected = sel;
        m_Lock->unlock();
        if(m_Selected) {
            // notify the VME selection.
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(mafCore::mafObjectBase *, this));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
        }
    }
}

mafDataSetCollection *mafVME::dataSetCollection() {
    if(m_DataSetCollection == NULL) {
        QWriteLocker locker(m_Lock);
        m_DataSetCollection = new mafDataSetCollection(mafCodeLocation);

        //connect the data collection modified to the updateBounds slot
        connect(m_DataSetCollection, SIGNAL(modifiedObject()), this, SLOT(updateBounds()));
    }
    return m_DataSetCollection;
}

mafDataSet *mafVME::outputData() {
    QReadLocker locker(m_Lock);
    if(m_DataPipe != NULL) {
        return m_DataPipe->output()->dataSetCollection()->itemAtCurrentTime();
    }
    return m_DataSetCollection->itemAtCurrentTime();
}

mafMemento *mafVME::createMemento() const {
    mafMemento *m = Superclass::createMemento();
    mafMementoVME *mementoVME = new mafMementoVME(this, true, mafCodeLocation);
    m->setParent(mementoVME);

    return mementoVME;
}

void mafVME::setMemento(mafMemento *memento, bool binary, bool deep_memento) {
    m_Binary = binary;
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    mafMemento *m = (mafMemento *)memento->children().at(0);
    Superclass::setMemento(m, deep_memento);

    //If not exists, creates the mafDataSetCollection
    if (m_DataSetCollection == NULL) {
        m_DataSetCollection = this->dataSetCollection();
    }

    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    int i = 0;
    for ( ; i < list->size(); ++i) {
        item = list->at(i);
        if(item.m_Name == "mafDataSetTime") {
            double time = item.m_Value.toDouble();
            mafMementoDataSet *mementoDataSet = mafNEW(mafResources::mafMementoDataSet);
            mafMementoPropertyList *propList = mementoDataSet->mementoPropertyList();

            item = list->at(i+1);
            if (item.m_Name == "poseMatrix") {
                propList->append(item);
                ++i; //
            }
            //append codecType, dataType, dataHash, dataSize and dataValue.
            int n = 0;
            for ( ; n < 5; ++n) {
                ++i;
                propList->append(list->at(i));
            }
            m_MementoDataSetHash.insert(mementoDataSet, time);

        } else if(item.m_Name == "mafPipeData") {
            this->setDataPipe(item.m_Value.toString());
        } else if(item.m_Name == "vmeBounds") {
            this->setBounds(item.m_Value.toList());
        }
    }
    setModified();
    execute();
}

void mafVME::updateData() {
    QHash<mafMementoDataSet *, double>::iterator iter = m_MementoDataSetHash.begin();
    while(iter != m_MementoDataSetHash.end()) {
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafMementoDataSet *memento = iter.key();
        memento->setObjectClassType(dataSet->metaObject()->className());
        dataSet->setMemento(memento, m_Binary);
        this->dataSetCollection()->insertItem(dataSet, iter.value());
        mafDEL(dataSet);
        ++iter;
    }
    this->setDataLoaded(true);
    this->updateBounds();
}

void mafVME::updateBounds() {
    if (this->dataSetCollection()->itemAtCurrentTime() != NULL){
        mafDataBoundaryAlgorithm *boundary = NULL;
        boundary = this->dataSetCollection()->itemAtCurrentTime()->boundaryAlgorithm();
        if(boundary != NULL){
            double b[6];
            boundary->bounds(b);
            int i = 0;
            for(; i < 6; ++i) {
                m_Bounds.append(b[i]);
            }
        }
    }
}

void mafVME::setVisibility(bool visible) {
  m_Visibility = visible;
}
