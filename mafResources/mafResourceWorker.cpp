/*
 *  mafResourceWorker.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourceWorker.h"
#include "mafResource.h"

using namespace mafCore;
using namespace mafResources;

mafResourceWorker::mafResourceWorker(const QString code_location) : mafObjectBase(code_location) {
}

mafResourceWorker::mafResourceWorker(mafResource *res, const QString code_location) : mafObjectBase(code_location), m_Resource(res) {
    m_ExecutionThread = new QThread();

    connect(m_ExecutionThread, SIGNAL(started()), m_Resource, SLOT(execute()));
    connect(this, SIGNAL(abortFlag()), m_Resource, SLOT(abortExecution()));
    connect(m_Resource, SIGNAL(executionEnded()), this, SIGNAL(workDone()));

    m_Resource->moveToThread(m_ExecutionThread);
}

mafResourceWorker::~mafResourceWorker() {
    mafDELThread(m_ExecutionThread);
}

void mafResourceWorker::doWork() {
    m_ExecutionThread->start();
}
