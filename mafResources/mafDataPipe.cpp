/*
 *  mafDataPipe.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataPipe.h"
#include "mafDataSet.h"
#include "mafVME.h"

using namespace mafResources;

mafDataPipe::mafDataPipe(const mafString code_location) : mafPipe(code_location), m_DecoratorPipe(NULL), m_Output(NULL), m_InPlace(true) {
}

mafDataPipe::~mafDataPipe() {
    if (!m_InPlace) {
        mafDEL(m_Output);
    }
}

int mafDataPipe::addInput(mafVME *vme) {
    REQUIRE(vme != NULL);
    if(this->inputList()->contains(vme)) {
        mafMsgWarning("%s", mafTr("Object %1 already present in input list").arg(vme->objectName()).toAscii().data());
        return this->inputList()->indexOf(vme);
    }

    // connect the input data destroyed to the inputDestroyed slot
    connect(vme, SIGNAL(destroyed()), this, SLOT(inputDestroyed()));

    this->inputList()->append(vme);
    return this->inputList()->count() - 1;
}


void mafDataPipe::inputDestroyed() {
    mafVME *vme = (mafVME *)QObject::sender();
    removeInput(vme);
}

