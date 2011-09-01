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

mafOperation::mafOperation(const QString code_location) : mafResource(code_location), m_Status(mafOperationStatusIdle), m_CanUnDo(true), m_CanAbort(true), m_InputPreserve(true), m_MultiThreaded(true) {
    setObjectName(this->metaObject()->className());
    connect(this, SIGNAL(executionEnded()), this, SLOT(terminate()));
}

mafOperation::~mafOperation() {
}

bool mafOperation::initialize() {
    m_Status = mafOperationStatusStarted;
    return true;
}

void mafOperation::setParameters(const QVariantMap &parameters) {
    QMapIterator<QString, QVariant> i(parameters);
    while (i.hasNext()) {
        i.next();
        this->setProperty(i.key().toAscii().constData(), i.value()); 
    } 

}

void mafOperation::terminate() {
    if (m_Status == mafOperationStatusExecuting) {
        m_Status = mafOperationStatusIdle;
    } else if (m_Status == mafOperationStatusStarted) {
        m_Status = mafOperationStatusCanceled;
    }
    terminated();
}

void mafOperation::abort() {
    m_Status = mafOperationStatusAborted;
}

void mafOperation::unDo() {

}

void mafOperation::reDo() {

}

