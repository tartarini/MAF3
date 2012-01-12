/*
 *  mafPipe.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipe.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafPipe::mafPipe(const QString code_location) : mafObject(code_location), m_InputList(NULL) {
    // Initialize the input list
    m_InputList = new mafVMEList();
    // and make a connection to update the pipe each time that a modify signal is emitted.
    connect(this, SIGNAL(modifiedObject()), this, SLOT(updatePipe()));
}

mafPipe::~mafPipe() {
    m_InputList->clear();
    m_ParametersHash.clear();
    
    delete m_InputList;
    m_InputList = NULL;
}

void mafPipe::setInput(mafVME *vme) {
    // Initialize to false the flag that establish if the pipe needs to be updated.
    bool needsUpdate(false);
    if(!m_InputList->isEmpty()) {
        // Input is already present. Try to change the pipe's input.
        const mafVME *input_vme = m_InputList->at(0);
        if (input_vme->isEqual(vme)) {
            // The given VME is equal to the first input. Do nothing.
            return;
        }
        // Remove old input and set the flag for update request to true.
        removeInput(0);
        needsUpdate = true;
    }
    
    if(vme != NULL) {
        // A new VME has to be added as input.
        m_InputList->append(vme);

        // Connect the pipe with the DataSet collection of the VME so to be updated when it is modified.
        mafDataSetCollection *dsCollection = vme->dataSetCollection();
        //connect the data collection modified to the updatePipe slot
        connect(dsCollection, SIGNAL(modifiedObject()), this, SLOT(updatePipe()));
    }
    
    // Don't send modifyObject notification the first time you set the input
    if (needsUpdate) {
        setModified();
    }
}

mafVME *mafPipe::input(int idx) const {
    if (idx < 0 || idx >= m_InputList->count()) {
        // NULL is returned for index outside the input list bounds.
        return NULL;
    }
    
    return m_InputList->at(idx);
}

mafDataSet *mafPipe::dataSetForInput(int input_idx, double t) {
    mafVME *vme = input(input_idx);
    if (vme == NULL) {
        return NULL;
    }
    
    // Return the mafDataSet at the given time.
    return vme->dataSetCollection()->itemAt(t);
}

void mafPipe::removeInput(mafVME *vme) {
    REQUIRE(vme != NULL);

    // Find the index of the given VME in the input list.
    int idx = m_InputList->indexOf(vme);
    if(idx != -1) {
        // Remove the found VME.
        m_InputList->removeAt(idx);
    } else {
        QByteArray ba = mafTr("Object %1 not present in input list").arg(vme->objectName()).toAscii();
        qWarning("%s", ba.data());
    }
}

void mafPipe::removeInput(const int idx) {
    REQUIRE(idx >= 0);

    if(idx < m_InputList->count()) {
        // Remove the input VME at given index.
        mafVME *vme = m_InputList->at(idx);
        m_InputList->removeAt(idx);
    } else {
        QByteArray ba = mafTr("Index %1 outside input list range.").arg(idx).toAscii();
        qWarning("%s", ba.data());
    }
}

void mafPipe::setParametersHash(const QVariantHash hash) {
    // Fuse parameters coming from the given hash with that one already present.
    m_ParametersHash.unite(hash);
    QVariantHash::iterator iter = m_ParametersHash.begin();
    while (iter != m_ParametersHash.end()) {
        // Cycle over all hash items and set parameters present as properties.
        QByteArray ba = iter.key().toAscii();
        QVariant val(iter.value());
        setProperty(ba.constData(), val);
        ++iter;
    }
}
