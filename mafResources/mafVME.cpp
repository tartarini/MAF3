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
#include "mafDataPipe.h"
#include "mafDataSet.h"
#include "mafMementoDataSet.h"
#include "mafInteractor.h"
#include "mafDataBoundaryAlgorithm.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafVME::mafVME(const mafString code_location) : mafResource(code_location), m_Interactor(NULL), m_DataSetCollection(NULL), m_DataPipe(NULL) {
    mafId time_set_id = mafIdProvider::instance()->idValue("TIME_SET");
    if(time_set_id != -1) {
        mafRegisterLocalCallback("TIME_SET", this, "setTimestamp(double)");
    }
    m_MementoDataSetHash.clear();
    connect(this, SIGNAL(modifiedObject()), this, SLOT(execute()));
}

mafVME::~mafVME() {
    mafDEL(m_Interactor);
    mafDEL(m_DataSetCollection);
    mafDEL(m_DataPipe);

    // delete all the created mementos for the dataset collection.
    mafHash<mafMementoDataSet *, double>::iterator iter = m_MementoDataSetHash.begin();
    while(iter != m_MementoDataSetHash.end()) {
        mafMementoDataSet *memento = iter.key();
        mafDEL(memento);
        ++iter;
    }
    m_MementoDataSetHash.clear();
}

void mafVME::setBounds(QVariantList bounds) {
    m_Bounds.clear();
    m_Bounds.append(bounds);
}


void mafVME::setTimestamp(double t) {
    dataSetCollection()->setTimestamp(t);
    setModified();
    execute();
}

void mafVME::setInteractor(mafInteractor *i) {
    if(i == m_Interactor) {
        return;
    }
    mafDEL(m_Interactor);
    m_Interactor = i;
}

void mafVME::setDataPipe(const mafString &pipe_type) {
    mafObjectBase *obj = mafNEWFromString(pipe_type);
    mafDataPipe *new_pipe = qobject_cast<mafDataPipe *>(obj);
    if(new_pipe) {
        setDataPipe(new_pipe);
        return;
    } else {
        mafMsgWarning("%s", mafTr("%1 does not represent a type of mafDataPipe.").arg(pipe_type).toAscii().data());
    }
    mafDEL(obj);
}

void mafVME::setDataPipe(mafDataPipe *pipe) {
    if(m_DataPipe == pipe) return;
    if(pipe) {
        pipe->setInput(this);
        mafDEL(m_DataPipe);
        m_DataPipe = pipe;
        setModified();
        execute();
    }
}

void mafVME::execute() {
    if(m_DataPipe) {
        double timeStamp = dataSetCollection()->timeStamp();
        m_DataPipe->output(timeStamp);
    }
}

void mafVME::detatch() {
    emit(detatched());
}

void mafVME::setSelected(bool sel) {
    if(m_Selected != sel) {
        m_Selected = sel;
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
        m_DataSetCollection = new mafDataSetCollection(mafCodeLocation);

        //connect the data collection modified to the updateBounds slot
        connect(m_DataSetCollection, SIGNAL(modifiedObject()), this, SLOT(updateBounds()));
    }
    return m_DataSetCollection;
}

mafDataSet *mafVME::outputData() {
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

void mafVME::setMemento(mafMemento *memento, bool deep_memento) {
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

        } else if(item.m_Name == "mafDataPipe") {
            this->setDataPipe(item.m_Value.toString());
        } else if(item.m_Name == "vmeBounds") {
            this->setBounds(item.m_Value.toList());
        }
    }
    setModified();
    execute();
}

void mafVME::updateData() {
    mafHash<mafMementoDataSet *, double>::iterator iter = m_MementoDataSetHash.begin();
    while(iter != m_MementoDataSetHash.end()) {
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafMementoDataSet *memento = iter.key();
        memento->setObjectClassType(dataSet->metaObject()->className());
        dataSet->setMemento(memento);
        this->dataSetCollection()->insertItem(dataSet, iter.value());
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
