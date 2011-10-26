/*
 *  mafOperation.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperation.h"

using namespace mafCore;
using namespace mafResources;

mafOperation::mafOperation(const QString code_location) : mafResource(code_location), m_Status(mafOperationStatusIdle), m_CanUnDo(true), m_CanAbort(true), m_InputPreserve(true), m_MultiThreaded(true) {
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
    QByteArray ba;
    while (i.hasNext()) {
        i.next();
        ba = i.key().toAscii();
        this->setProperty(ba.constData(), i.value()); 
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

void mafOperation::fillDictionary() {
    Superclass::fillDictionary();
    
    //here insert custom parameters
    dictionary()->insert("Multithread", isMultiThreaded());
}

