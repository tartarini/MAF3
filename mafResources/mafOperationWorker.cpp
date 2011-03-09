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

mafOperationWorker::mafOperationWorker(const QString code_location) : mafObjectBase(code_location) {
}

mafOperationWorker::mafOperationWorker(mafOperation *op, const QString code_location) : mafObjectBase(code_location), m_Operation(op) {
    m_ExecutionThread = new QThread();
    m_Operation->moveToThread(m_ExecutionThread);
    m_Operation->ref();

    initializeWorkflow();
}

mafOperationWorker::~mafOperationWorker() {
    m_StateMachine.stop();
    mafDEL(m_Operation);
    mafDELThread(m_ExecutionThread);
}

void mafOperationWorker::initializeWorkflow() {
    connect(m_ExecutionThread, SIGNAL(started()), m_Operation, SIGNAL(startExecution()));
    connect(m_Operation, SIGNAL(executionEnded()), this, SIGNAL(workDone()));
    connect(this, SIGNAL(workAborted()), m_Operation, SLOT(abort()));
    connect(this, SIGNAL(workAborted()), m_Operation, SLOT(terminate()));

    // Create the state machine's states.
    QState *idleState = new QState(&m_StateMachine);
    idleState->setObjectName("idle");
    QState *runningState = new QState(&m_StateMachine);
    runningState->setObjectName("running");
    QState *executedState = new QState(&m_StateMachine);
    executedState->setObjectName("executed");
    QState *stackedState = new QState(&m_StateMachine);
    stackedState->setObjectName("stacked");
    QFinalState *finalState = new QFinalState(&m_StateMachine);
    finalState->setObjectName("final");

    // Connect the state entered/exited with operation's slots
    connect(runningState, SIGNAL(entered()), m_Operation, SLOT(execute()));

    // Add transitions.
    idleState->addTransition(m_Operation, SIGNAL(startExecution()), runningState);
    runningState->addTransition(m_Operation, SIGNAL(executionEnded()), executedState);
    if ( !m_Operation->canUnDo() ) {
        executedState->addTransition(m_Operation, SIGNAL(executionEnded()), finalState);
    } else {
        executedState->addTransition(m_Operation, SIGNAL(undoExecution()), stackedState);
    }

    stackedState->addTransition(m_Operation, SIGNAL(startExecution()), runningState);

    idleState->addTransition(m_Operation, SIGNAL(terminateExecution()), finalState);
    stackedState->addTransition(m_Operation, SIGNAL(terminateExecution()), finalState);

    // Set the initial state of the state machine
    m_StateMachine.setInitialState(idleState);

    // Start the stat machine.
    m_StateMachine.start();
}

void mafOperationWorker::doWork() {
    m_ExecutionThread->start();
}

void mafOperationWorker::abortExecution() {
    if ( m_Operation->canAbort() ) {
        m_ExecutionThread->quit();
        emit workAborted();
    }
}
