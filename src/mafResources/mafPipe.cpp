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
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafPipe::mafPipe(const QString code_location) : mafObject(code_location), m_InputList(NULL) {
    m_InputList = new mafVMEList();
}

mafPipe::~mafPipe() {
    m_InputList->clear();
    delete m_InputList;
    m_InputList = NULL;
}

void mafPipe::setInput(mafVME *vme) {
    REQUIRE(vme != NULL);
    if(m_InputList->isEmpty()) {
        m_InputList->append(vme);
    } else {
        m_InputList->replace(0, vme);
    }

    // connect the input data destroyed to the inputDestroyed slot
    connect(vme, SIGNAL(destroyed()), this, SLOT(inputDestroyed()));
}

void mafPipe::removeInput(mafVME *vme) {
    REQUIRE(vme != NULL);

    int idx = m_InputList->indexOf(vme);
    if(idx != -1) {
        m_InputList->removeAt(idx);
    } else {
        qWarning("%s", mafTr("Object %1 not present in input list").arg(vme->objectName()).toAscii().data());
    }
}

void mafPipe::removeInput(const int idx) {
    REQUIRE(idx >= 0);

    if(idx < m_InputList->count()) {
        mafVME *vme = m_InputList->at(idx);
        disconnect(vme, SIGNAL(destroyed()),this, SLOT(inputDestroyed()));
        m_InputList->removeAt(idx);
    } else {
        qWarning("%s", mafTr("Index %1 outside input list range.").arg(idx).toAscii().data());
    }
}


void mafPipe::inputDestroyed() {
    mafVME *vme = (mafVME *)QObject::sender();
    m_InputList->removeAt(m_InputList->indexOf(vme));
    disconnect(vme, SIGNAL(destroyed()),this, SLOT(inputDestroyed()));
}
