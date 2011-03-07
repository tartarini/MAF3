/*
 *  mafOperationManager.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi on 05/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationManager.h"
#include "mafContracts.h"
#include "mafOperation.h"
#include "mafResourcesRegistration.h"
#include "mafVME.h"
#include "mafUndoStackCommand.h"
#include "mafResourceWorker.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafOperationManager* mafOperationManager::instance() {
    // Create the instance of the VME manager.
    static mafOperationManager instanceOperationManager;
    return &instanceOperationManager;
}

void mafOperationManager::shutdown() {
    while ( m_ExecutionPool.size() > 0 ) {
        // Wait that all threaded operations terminates...
    }
    flushUndoStack();
}

mafOperationManager::mafOperationManager(const QString code_location) : mafObjectBase(code_location), m_CurrentOperation(NULL) {
    initializeConnections();

    //request of the selected vme
    mafCore::mafObjectBase *sel_vme;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);
    vmeSelect(sel_vme);
}

mafOperationManager::~mafOperationManager() {
}

void mafOperationManager::initializeConnections() {
    // Create the IDs required to add a resource to the management system.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.resources.operation.start");
    provider->createNewId("maf.local.resources.operation.setParameters");
    provider->createNewId("maf.local.resources.operation.execute");
    provider->createNewId("maf.local.resources.operation.executeWithParameters");
    provider->createNewId("maf.local.resources.operation.stop");
    provider->createNewId("maf.local.resources.operation.undo");
    provider->createNewId("maf.local.resources.operation.redo");
    provider->createNewId("maf.local.resources.operation.updateUndoStack");
    provider->createNewId("maf.local.resources.operation.sizeUndoStack");
    provider->createNewId("maf.local.resources.operation.currentRunning");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.operation.start", this, "startOperationSignal(const QString)")
    mafRegisterLocalSignal("maf.local.resources.operation.started", this, "operationDidStart(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.operation.setParameters", this, "setOperationParametersSignal(QVariantList)")
    mafRegisterLocalSignal("maf.local.resources.operation.execute", this, "executeOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.executeWithParameters", this, "executeWithParametersSignal(QVariantList)")
    mafRegisterLocalSignal("maf.local.resources.operation.stop", this, "stopOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.undo", this, "undoOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.redo", this, "redoOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.updateUndoStack", this, "updateUndoStackSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.operation.sizeUndoStack", this, "undoStackSizeSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.currentRunning", this, "currentOperationSignal()")

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.operation.start", this, "startOperation(const QString)")
    mafRegisterLocalCallback("maf.local.resources.operation.setParameters", this, "setOperationParameters(QVariantList)")
    mafRegisterLocalCallback("maf.local.resources.operation.execute", this, "executeOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.executeWithParameters", this, "executeWithParameters(QVariantList)")
    mafRegisterLocalCallback("maf.local.resources.operation.stop", this, "stopOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.undo", this, "undoOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.redo", this, "redoOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.updateUndoStack", this, "updateUndoStack(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.operation.sizeUndoStack", this, "undoStackSize()")
    mafRegisterLocalCallback("maf.local.resources.operation.currentRunning", this, "currentOperation()")

    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}


void mafOperationManager::vmeSelect(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafResources::mafVME*>(obj);
    if(vme && vme != m_SelectedVME) {
        m_SelectedVME = vme;
    }
}

void mafOperationManager::executeWithParameters(QVariantList op_with_parameters) {
    REQUIRE(op_with_parameters.count() == 2);
    //parameters contains as first argument the operation name
    QString op_to_run = op_with_parameters.at(0).toString();
    REQUIRE(!op_to_run.isEmpty());

    this->startOperation(op_to_run);
    //parameters contains as second argument a list of QVariant which are passed to the operation
    m_CurrentOperation->setParameters(op_with_parameters.at(1).toList());
    this->executeOperation();
}

void mafOperationManager::setOperationParameters(QVariantList parameters) {
    REQUIRE(m_CurrentOperation);
    m_CurrentOperation->setParameters(parameters);
}

void mafOperationManager::startOperation(const QString operation) {
    REQUIRE(!operation.isEmpty());
//    REQUIRE(m_CurrentOperation == NULL); //require also that there isn't running operation

    // Create the instance of the new operation to execute.
    m_CurrentOperation = (mafOperation *)mafNEWFromString(operation);

    if(m_CurrentOperation == NULL) {
        qWarning() << mafTr("Operation type '%1' not created. It needs to be register into the mafObjectFactory!!").arg(operation);
        return;
    }

    // Assign as input the current selected VME.
    m_CurrentOperation->setInput(m_SelectedVME);
    // and ask the operation to initialize itself.
    bool result = m_CurrentOperation->initialize();

    // Notify the observers that the new operation has started.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase*, m_CurrentOperation));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.started", mafEventTypeLocal, &argList);

    ENSURE(result);
}

void mafOperationManager::executeOperation() {
    if ( m_CurrentOperation != NULL ) {
        // put it into the undo stack.
        m_UndoStack << m_CurrentOperation;

        // Create a resource worker and pass to it the resource to be execute in a separate thread.
        mafResourceWorker *worker = new mafResourceWorker(m_CurrentOperation, mafCodeLocation);
        // become observer wo be notified when the work is done.
        connect(worker, SIGNAL(workDone()), this, SLOT(operationExecuted()));
        connect(worker, SIGNAL(abortFlag()), this, SLOT(stopOperation()));
        // Put the worker into the pool.
        m_ExecutionPool << worker;
        // Start the work.
        worker->doWork();
//            connect(m_CurrentOperation, SIGNAL(executionEnded()), m_ExecutionThread, SLOT(quit()));
    }
}

mafResourceWorker *mafOperationManager::removeWorkerFromPool(QObject *obj) {
    mafResourceWorker *worker = qobject_cast<mafResources::mafResourceWorker *>(obj);
    if (worker) {
        int idx = m_ExecutionPool.indexOf(worker);
        if ( idx != -1 ) {
            m_ExecutionPool.remove(idx);
        }
    }
    return worker;
}

void mafOperationManager::operationExecuted() {
    // Remove the resource worker from the list.
    mafResourceWorker *worker = removeWorkerFromPool(QObject::sender());

    mafResource *res = worker->resource();
    mafOperation *op = qobject_cast<mafResources::mafOperation *>(res);

    REQUIRE(op != NULL);

    if ( !op->canUnDo() ) {
        updateUndoStack(op);
        mafDEL(op);
    } else {
        int idx = m_UndoStack.indexOf(op);
        if ( idx == -1 ) {
            mafDEL(op);
        }
    }

    mafDEL(worker);
}

void mafOperationManager::stopOperation() {
    // Remove the resource worker from the list.
    mafResourceWorker *worker = removeWorkerFromPool(QObject::sender());

    if ( worker == NULL ) {
        // Operation not executing => Simply cancelled by the user.
        // m_CurrentOperation hsould be deleted.
        mafDEL(m_CurrentOperation);
        return;
    }

    mafResource *res = worker->resource();
    mafOperation *op = qobject_cast<mafResources::mafOperation *>(res);

    REQUIRE(op != NULL);

    op->terminate();
    int idx = m_UndoStack.indexOf(op);
    if ( idx != -1 ) {
        m_UndoStack.removeAt(idx);
    }
    mafDEL(op);
    mafDEL(worker);
}

void mafOperationManager::undoOperation() {
//    mafUndoStackCommand usc(m_LastExecutedOperation, "unDo");
//    usc.execute();
//    QLinkedList<mafOperation*>::Iterator i;
//    for(i = m_UndoStack.begin() ; i != m_UndoStack.end(); ++i) {
//        if(*i == m_LastExecutedOperation) {
//            if(i == m_UndoStack.begin()) {
//                m_LastExecutedOperation = NULL;
//            } else {
//                m_LastExecutedOperation = *(--i);
//            }
//            break;
//        }
//    }
}

void mafOperationManager::redoOperation() {
//    QLinkedList<mafOperation*>::Iterator i;
//    for(i = m_UndoStack.begin() ; i != m_UndoStack.end(); ++i) {
//        if(m_LastExecutedOperation == NULL) { //keep separate from below condition only for undertanding better
//            //means that with undo, user arrives at the beginning
//            m_LastExecutedOperation = *(m_UndoStack.begin());
//            mafUndoStackCommand usc(m_LastExecutedOperation, "reDo");
//            usc.execute();
//            //m_LastExecutedOperation->reDo();
//            break;
//        }
//        if(*i == m_LastExecutedOperation) {
//            if(++i == m_UndoStack.end()) {
//                qWarning("%s", mafTr("Can't redo, undo stack finished").toAscii().data());
//            } else {
//                m_LastExecutedOperation = *i;
//                mafUndoStackCommand usc(m_LastExecutedOperation, "reDo");
//                usc.execute();
//                //m_LastExecutedOperation->reDo();
//                break;
//            }
//        }
//    }
}

void mafOperationManager::updateUndoStack(mafCore::mafObjectBase *no_undo_op) {
    QList<mafOperation*>::iterator iter = m_UndoStack.begin();
    mafOperation *op;
    foreach(op, m_UndoStack) {
        ++iter;
        if (op == no_undo_op) {
            break;
        }
    }
    m_UndoStack.erase(m_UndoStack.begin(), iter);
}

void mafOperationManager::flushUndoStack() {
    mafOperation *op;
    foreach(op, m_UndoStack) {
        mafDEL(op);
    }
    m_UndoStack.clear();
}
