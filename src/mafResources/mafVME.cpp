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

mafVME::mafVME(const QString code_location) : mafResource(code_location), m_Interactor(NULL), m_DataSetCollection(NULL), m_DataPipe(NULL), m_CanRead(true), m_CanWrite(true) {
    m_UIFilename = "vmeGeneric.ui";
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
    if ( i == m_Interactor ) {
        return;
    }
    emit interactorDetach();
    m_Lock->lockForWrite();
    m_Interactor = i;
    m_Lock->unlock();
    emit interactorAttached();
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
        //execute();
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

mafDataSetCollection *mafVME::dataSetCollection() {
    if(m_DataSetCollection == NULL) {
        m_DataSetCollection = new mafDataSetCollection(mafCodeLocation);
        m_DataSetCollection->setParent(this);
        //connect the data collection modified to the updateBounds slot
    }
    return m_DataSetCollection;
}

mafDataSet *mafVME::outputData() {
    QReadLocker locker(m_Lock);
    if(m_DataPipe != NULL) {
        return m_DataPipe->output()->dataSetCollection()->itemAtCurrentTime();
    }
    return dataSetCollection()->itemAtCurrentTime();
}

mafMemento *mafVME::createMemento() const {
    mafMemento *m = Superclass::createMemento();
    mafMementoVME *mementoVME = new mafMementoVME(this, true, mafCodeLocation);
    m->setParent(mementoVME);

    return mementoVME;
}

void mafVME::setMemento(mafMemento *memento, bool deep_memento) {
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    // save selected node
    mafCore::mafObjectBase *sel_vme;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

    // add vme
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, this));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    // Notify the item selection.
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase*, this));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
    

    int n = 0;
    int childrenNum = memento->children().size();
    for (n; n < childrenNum; n++) {
      mafMemento *m = (mafMemento *)memento->children().at(n);
      if (m->serializationPattern() == mafSerializationPatternInheritance) {
        //set the memento of the superclass
        Superclass::setMemento(m, deep_memento);
        int i = 0;
      } else {
        //set the memento of the children memento
        QString objClassType = m->objectClassType();
        mafObjectBase *objBase = mafNEWFromString(objClassType);
        mafObject *obj = qobject_cast<mafCore::mafObject *>(objBase);

        obj->setMemento(m, deep_memento);
        if (objClassType == "mafResources::mafDataSetCollection") {
          mafDataSetCollection *dataSetCollection = qobject_cast<mafDataSetCollection*>(obj);
          m_DataSetCollection = dataSetCollection;
        } else {
            mafDEL(objBase);
        }
      }
    }

    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    //create a map of memento dataset
    int i = 0;
    for ( ; i < list->size(); ++i) {
        item = list->at(i);
        if(item.m_Name == "mafPipeData") {
            this->setDataPipe(item.m_Value.toString());
        }
    }
    setModified();
    execute();

    // Select parent node.
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase*, sel_vme));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
}


bool mafVME::dataLoaded() const {
    bool result(false);
    if(m_DataSetCollection){
        result = m_DataSetCollection->itemAtCurrentTime()->dataLoaded();
    }
    return result;
}

QString mafVME::boundXmin() {
    return QString::number(this->dataSetCollection()->itemAtCurrentTime()->bounds()[0].toDouble());
}

QString mafVME::boundXmax() {
    return QString::number(this->dataSetCollection()->itemAtCurrentTime()->bounds()[1].toDouble());
}

QString mafVME::boundYmin() {
    return QString::number(this->dataSetCollection()->itemAtCurrentTime()->bounds()[2].toDouble());
}

QString mafVME::boundYmax() {
    return QString::number(this->dataSetCollection()->itemAtCurrentTime()->bounds()[3].toDouble());
}

QString mafVME::boundZmin() {
    return QString::number(this->dataSetCollection()->itemAtCurrentTime()->bounds()[4].toDouble());
}

QString mafVME::boundZmax() {
    return QString::number(this->dataSetCollection()->itemAtCurrentTime()->bounds()[5].toDouble());
}


