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

using namespace mafCore;
using namespace mafResources;

mafVME::mafVME(const mafString code_location) : mafResource(code_location), m_Modified(false), m_Interactor(NULL), m_DataSetCollection(NULL), m_DataPipe(NULL) {
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
        iter++;
    }
    m_MementoDataSetHash.clear();
}

void mafVME::setModified(bool m) {
    m_Modified = m;

    if(!m_Modified) {
        return;
    }

    // Connect this modifiedObject signal with the class that hsould listen it
    // like the mafDataPipe or mafVisualPipe
    emit(modifiedObject());
}

void mafVME::setTimestamp(double t) {
    dataSetCollection()->setTimestamp(t);
    setModified();
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
    mafDataPipe *new_pipe = dynamic_cast<mafDataPipe *>(obj);
    if(new_pipe) {
        setDataPipe(new_pipe);
        return;
    } else {
        mafMsgWarning("%s", mafTr("%1 does not represent a type of mafDataPipe.").arg(pipe_type).toAscii().data());
    }
    mafDEL(obj);
}

void mafVME::setDataPipe(mafDataPipe *pipe) {
    mafDEL(m_DataPipe);
    m_DataPipe = pipe;
    setModified();
}

void mafVME::execute() {
    if(m_DataPipe) {
        m_DataPipe->updatePipe(dataSetCollection()->timeStamp());
    }
}

mafDataSet *mafVME::outputData() {
    if(m_DataPipe != NULL) {
        return m_DataPipe->output();
    }
    return m_DataSetCollection->itemAtCurrentTime();
}

mafMemento *mafVME::createMemento() const {
    mafMemento *m = Superclass::createMemento();
    mafMementoVME *mementoVME = new mafMementoVME(this, m_DataSetCollection, m_DataPipe, mafCodeLocation);
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
                i++; //
            }
            //append codecType, dataType, dataHash, dataSize and dataValue.
            int n = 0;
            for ( ; n < 5; n++) {
                i++;
                propList->append(list->at(i));
            }
            m_MementoDataSetHash.insert(mementoDataSet, time);

        } else if(item.m_Name == "mafDataPipe") {
            this->setDataPipe(item.m_Value.toString());
        }
    }
    setModified();
}

void mafVME::updateData() {
    mafHash<mafMementoDataSet *, double>::iterator iter = m_MementoDataSetHash.begin();
    while(iter != m_MementoDataSetHash.end()) {
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafMementoDataSet *memento = iter.key();
        memento->setObjectClassType(dataSet->metaObject()->className());
        dataSet->setMemento(memento);
        this->dataSetCollection()->insertItem(dataSet, iter.value());
        iter++;
    }
    this->setDataLoaded(true);
}
