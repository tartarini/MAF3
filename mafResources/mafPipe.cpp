/*
 *  mafPipe.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipe.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"

using namespace mafCore;
using namespace mafResources;

mafPipe::mafPipe(const mafString code_location) : mafObject(code_location), m_InputList(NULL) {
    m_InputList = new mafDataSetList();
}

mafPipe::~mafPipe() {
    m_InputList->clear();
    mafDEL(m_InputList);
}

int mafPipe::setInput(mafDataSet *data, const int idx) {
    REQUIRE(data != NULL);
    REQUIRE(idx >= 0);

    if(m_InputList->contains(data)) {
        mafMsgWarning("%s", mafTr("Object %1 already present in input list").arg(data->objectName()).toAscii().data());
        return m_InputList->indexOf(data);
    }

    if (data->isObjectValid()) {
        // connect the input data destroyed to the inputDestroyed slot
        connect(data, SIGNAL(destroyed()), this, SLOT(inputDestroyed()));

        // Add the new data to the input list.
        if(idx >= m_InputList->size()) {
            m_InputList->append(data);
            return m_InputList->count() - 1;
        } else {
            removeInput(idx);
            m_InputList->insert(idx, data);
            return idx;
        }
    }

    mafMsgWarning("%s", mafTr("Some problem occourred during data insertion for index %1").arg(idx).toAscii().data());
    return -1;
}

int mafPipe::addInput(mafDataSet *data) {
    return setInput(data, m_InputList->count());
}

int mafPipe::addInputMulti(mafDataSetCollection *data) {
    REQUIRE(data);
    const mafDataSetMap *datasetList = data->collectionMap();
    mafDataSetMap::const_iterator iter = datasetList->constBegin();
    mafDataSetMap::const_iterator iter_end = datasetList->constEnd();
    int res(-1);
    while(iter != iter_end) {
        mafDataSet *d = iter.value();
        res = setInput(d, m_InputList->count());
        iter++;
    }
    return res;
}

void mafPipe::removeInput(const int idx) {
    REQUIRE(idx >= 0);

    if(idx < m_InputList->count()) {
        mafDataSet *data = m_InputList->at(idx);
        disconnect(data, SIGNAL(destroyed()),this, SLOT(inputDestroyed()));
        m_InputList->removeAt(idx);
    } else {
        mafMsgWarning("%s", mafTr("Index %1 outside input list range.").arg(idx).toAscii().data());
    }
}

void mafPipe::removeInput(mafDataSet *data) {
    REQUIRE(data != NULL);

    int idx = m_InputList->indexOf(data);
    if(idx != -1) {
        removeInput(idx);
    } else {
        mafMsgWarning("%s", mafTr("Object %1 not present in input list").arg(data->objectName()).toAscii().data());
    }
}

void mafPipe::inputDestroyed() {
    mafDataSet *data = (mafDataSet *)QObject::sender();
    removeInput(data);
}
