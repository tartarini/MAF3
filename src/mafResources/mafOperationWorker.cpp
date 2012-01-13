/*
 *  mafOperationWorker.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationWorker.h"
#include "mafOperation.h"

using namespace mafCore;
using namespace mafResources;

mafOperationWorker::mafOperationWorker() {
}

mafOperationWorker::mafOperationWorker(mafOperation *op) :  m_Operation(op) {
    // Retain the operation reference count.
    m_Operation->retain();
    // Move the operation on the worker's thread and initialize the connections
    // between the worker thread's execution and operation's life time.
    m_Operation->moveToThread(this);
    initializeWorkflow();
}

mafOperationWorker::~mafOperationWorker() {
    qDebug() << mafTr("Destroying operation ") << m_Operation->objectName() << " ref: " << m_Operation->referenceCount();

    // Release the operation reference count.
    mafDEL(m_Operation);
}

void mafOperationWorker::initializeWorkflow() {
    connect(m_Operation, SIGNAL(executionEnded()), this, SIGNAL(workDone()), Qt::DirectConnection);

    connect(this, SIGNAL(workAborted()), m_Operation, SLOT(abort()), Qt::DirectConnection);
    connect(this, SIGNAL(workAborted()), m_Operation, SLOT(terminate()), Qt::DirectConnection);
}

void mafOperationWorker::run() {
   // Worker thread started...start the operation execution...
    m_Operation->execute();
}

void mafOperationWorker::abortExecution() {
    // Thread terminater by the user. If the operation can be aborted, abort it.
    if ( m_Operation->canAbort() ) {
        Q_EMIT workAborted();
    }
}
