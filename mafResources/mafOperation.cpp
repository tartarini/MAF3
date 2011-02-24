/*
 *  mafOperation.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperation.h"

using namespace mafCore;
using namespace mafResources;

mafOperation::mafOperation(const mafString code_location) : mafResource(code_location), m_IsRunning(false), m_CanUnDo(true), m_OperationType(mafOperationTypeOperation) {
    // to be moved into the mafOperationManager.
//    if(mafIdProvider::instance()->idValue("SET_PARAMETERS") == -1) {
//        mafId setParamId = mafIdProvider::instance()->createNewId("SET_PARAMETERS");
//        mafRegisterLocalSignal(setParamId, this, "setParametersSignal(mafList<mafVariant>)");
//        mafRegisterLocalCallback(setParamId, this, "setParameters(mafList<mafVariant>)");
//    }
}

mafOperation::~mafOperation() {
}

bool mafOperation::initialize() {
    m_IsRunning = true;
    return m_IsRunning;
}

void mafOperation::execute() {
}

bool mafOperation::terminate() {
    m_IsRunning = false;
    return m_IsRunning;
}

void mafOperation::unDo() {

}

void mafOperation::reDo() {

}

void mafOperation::setParameters(mafList<mafVariant> parameters) {
    Q_UNUSED(parameters);
}
