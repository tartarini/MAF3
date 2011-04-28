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

mafOperation::mafOperation(const QString code_location) : mafResource(code_location), m_Status(IDLE), m_CanUnDo(true), m_CanAbort(true), m_InputPreserve(true) {
    setObjectName(this->metaObject()->className());
    connect(this, SIGNAL(executionEnded()), this, SLOT(terminate()));
}

mafOperation::~mafOperation() {
}

bool mafOperation::initialize() {
    m_Status = STARTED;
    return true;
}

void mafOperation::terminate() {
    if (m_Status == EXECUTING) {
        m_Status = IDLE;
    } else if (m_Status == STARTED) {
        m_Status = CANCELED;
    }
    terminated();
}

void mafOperation::abort() {
    m_Status = ABORTED;
}

void mafOperation::unDo() {

}

void mafOperation::reDo() {

}

void mafOperation::setParameters(QVariantList parameters) {
    Q_UNUSED(parameters);
}
