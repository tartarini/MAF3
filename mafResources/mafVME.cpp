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

using namespace mafCore;
using namespace mafResources;

mafVME::mafVME(const mafString code_location) : mafResource(code_location), m_DataSetCollection(NULL), m_DataPipe(NULL) {
    mafId time_set_id = mafIdProvider::instance()->idValue("TIME_SET");
    if(time_set_id != -1) {
        mafRegisterLocalCallback("TIME_SET", this, "setTimestamp(double)");
    }
    m_MementoDatSetHash.clear();
}

mafVME::~mafVME() {
    mafDEL(m_DataSetCollection);
    mafDEL(m_DataPipe);
    mafHash<mafMementoDataSet *, double>::iterator iter = m_MementoDatSetHash.begin();
    while(iter != m_MementoDatSetHash.end()) {
        mafMementoDataSet *memento = iter.key();
        mafDEL(memento);
        iter++;
    }
    m_MementoDatSetHash.clear();
}

void mafVME::setTimestamp(double t) {
    dataSetCollection()->setTimestamp(t);

    // VME is now modified, should ask for an update.
    execute();
}

void mafVME::setDataPipe(const mafString &pipe_type) {
    mafObjectBase *obj = mafNEWFromString(pipe_type);
    mafDataPipe *new_pipe = dynamic_cast<mafDataPipe *>(obj);
    if(new_pipe) {
        setDataPipe(new_pipe);
    } else {
        mafMsgWarning("%s", mafTr("%1 does not represent a type of mafDataPipe.").arg(pipe_type).toAscii().data());
    }
}

void mafVME::setDataPipe(mafDataPipe *pipe) {
    REQUIRE(pipe != NULL);

    mafDEL(m_DataPipe);
    m_DataPipe = pipe;
}

bool mafVME::execute() {
    if(m_DataPipe) {
        m_DataPipe->updatePipe(dataSetCollection()->timeStamp());
    }
    return true;
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
            m_MementoDatSetHash.insert(mementoDataSet, time);

        } else if(item.m_Name == "mafDataPipe") {
            this->setDataPipe(item.m_Value.toString());
        }
    }
}

void mafVME::updateData() {
    mafHash<mafMementoDataSet *, double>::iterator iter = m_MementoDatSetHash.begin();
    while(iter != m_MementoDatSetHash.end()) {
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafMementoDataSet *memento = iter.key();
        memento->setObjectClassType(dataSet->metaObject()->className());
        dataSet->setMemento(memento);
        this->dataSetCollection()->insertItem(dataSet, iter.value());
        iter++;
    }
    this->setDataLoaded(true);
}
