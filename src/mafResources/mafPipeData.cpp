/*
 *  mafPipeData.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeData.h"
#include "mafDataSet.h"
#include "mafVME.h"

using namespace mafResources;

mafPipeData::mafPipeData(const QString code_location) : mafPipe(code_location), m_DecoratorPipe(NULL), m_Output(NULL), m_InPlace(true) {
}

mafPipeData::~mafPipeData() {
    if (!m_InPlace) {
        mafDEL(m_Output);
    }
    mafDEL(m_DecoratorPipe);
}

int mafPipeData::addInput(mafVME *vme) {
    REQUIRE(vme != NULL);
    if(this->inputList()->contains(vme)) {
        qWarning("%s", mafTr("Object %1 already present in input list").arg(vme->objectName()).toAscii().data());
        return this->inputList()->indexOf(vme);
    }

    // connect the input data destroyed to the inputDestroyed slot
    connect(vme, SIGNAL(destroyed()), this, SLOT(inputDestroyed()));

    this->inputList()->append(vme);
    setModified();
    return this->inputList()->count() - 1;
}


void mafPipeData::inputDestroyed() {
    mafVME *vme = (mafVME *)QObject::sender();
    removeInput(vme);
    setModified();
}

void mafPipeData::updatePipe(double t) {
    Q_UNUSED(t);
    //here update local output
    m_Output = inputList()->at(0);
}

mafVME *mafPipeData::output(double t) {
    REQUIRE(inputList()->count() > 0);

    if(m_DecoratorPipe) {
        updatePipe(t);
        return m_DecoratorPipe->output(t); 
    }

    if(m_Output == NULL || modified()) {
        updatePipe(t);
        emit(modifiedObject());
        setModified(false);
    }

    ENSURE(m_Output != NULL);
    return m_Output;
}
