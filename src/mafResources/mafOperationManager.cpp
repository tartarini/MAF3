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
#include "mafOperationWorker.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafOperationManager* mafOperationManager::instance() {
    // Create the instance of the VME manager.
    static mafOperationManager instanceOperationManager;
    return &instanceOperationManager;
}

void mafOperationManager::shutdown() {
    qDebug() << "Execution pool size: " << m_ExecutionPool.size();
//    while ( m_ExecutionPool.size() > 0 ) {
//        // Wait that all threaded operations terminates...
//    }
    flushUndoStack();
}

mafOperationManager::mafOperationManager(const QString code_location) : mafObjectBase(code_location), m_CurrentOperation(NULL), m_LastUndoneOperation(NULL) {
    initializeConnections();
}

mafOperationManager::~mafOperationManager() {
    // Unregister callbacks.
    mafUnregisterLocalCallback("maf.local.resources.operation.start", this, "startOperation(const QString)")
    mafUnregisterLocalCallback("maf.local.resources.operation.setParameters", this, "setOperationParameters(QVariantList)")
    mafUnregisterLocalCallback("maf.local.resources.operation.execute", this, "executeOperation()")
    mafUnregisterLocalCallback("maf.local.resources.operation.executeWithParameters", this, "executeWithParameters(QVariantList)")
    mafUnregisterLocalCallback("maf.local.resources.operation.stop", this, "stopOperation()")
    mafUnregisterLocalCallback("maf.local.resources.operation.undo", this, "undoOperation()")
    mafUnregisterLocalCallback("maf.local.resources.operation.redo", this, "redoOperation()")
    mafUnregisterLocalCallback("maf.local.resources.operation.sizeUndoStack", this, "undoStackSize()")
    mafUnregisterLocalCallback("maf.local.resources.operation.currentRunning", this, "currentOperation()")
    mafUnregisterLocalCallback("maf.local.resources.operation.executionPool", this, "executionPool()")
    
    // Unregister signals...
    mafUnregisterLocalSignal("maf.local.resources.operation.start", this, "startOperationSignal(const QString)")
    mafUnregisterLocalSignal("maf.local.resources.operation.started", this, "operationDidStart(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.operation.setParameters", this, "setOperationParametersSignal(QVariantList)")
    mafUnregisterLocalSignal("maf.local.resources.operation.execute", this, "executeOperationSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.executed", this, "executedOperationSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.executeWithParameters", this, "executeWithParametersSignal(QVariantList)")
    mafUnregisterLocalSignal("maf.local.resources.operation.stop", this, "stopOperationSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.undo", this, "undoOperationSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.redo", this, "redoOperationSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.sizeUndoStack", this, "undoStackSizeSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.currentRunning", this, "currentOperationSignal()")
    mafUnregisterLocalSignal("maf.local.resources.operation.executionPool", this, "executionPoolSignal()")

    mafIdProvider *provider = mafIdProvider::instance();
    provider->removeId("maf.local.resources.operation.start");
    provider->removeId("maf.local.resources.operation.setParameters");
    provider->removeId("maf.local.resources.operation.execute");
    provider->removeId("maf.local.resources.operation.executed");
    provider->removeId("maf.local.resources.operation.executeWithParameters");
    provider->removeId("maf.local.resources.operation.stop");
    provider->removeId("maf.local.resources.operation.stopped");
    provider->removeId("maf.local.resources.operation.undo");
    provider->removeId("maf.local.resources.operation.redo");
    provider->removeId("maf.local.resources.operation.sizeUndoStack");
    provider->removeId("maf.local.resources.operation.currentRunning");
    provider->removeId("maf.local.resources.operation.executionPool");
}

void mafOperationManager::initializeConnections() {
    // Create the IDs required to add a resource to the management system.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.resources.operation.start");
    provider->createNewId("maf.local.resources.operation.setParameters");
    provider->createNewId("maf.local.resources.operation.execute");
    provider->createNewId("maf.local.resources.operation.executed");
    provider->createNewId("maf.local.resources.operation.executeWithParameters");
    provider->createNewId("maf.local.resources.operation.stop");
    provider->createNewId("maf.local.resources.operation.stopped");
    provider->createNewId("maf.local.resources.operation.undo");
    provider->createNewId("maf.local.resources.operation.redo");
    provider->createNewId("maf.local.resources.operation.sizeUndoStack");
    provider->createNewId("maf.local.resources.operation.currentRunning");
    provider->createNewId("maf.local.resources.operation.executionPool");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.operation.start", this, "startOperationSignal(const QString)")
    mafRegisterLocalSignal("maf.local.resources.operation.started", this, "operationDidStart(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.operation.setParameters", this, "setOperationParametersSignal(QVariantList)")
    mafRegisterLocalSignal("maf.local.resources.operation.execute", this, "executeOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.executed", this, "executedOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.executeWithParameters", this, "executeWithParametersSignal(QVariantList)")
    mafRegisterLocalSignal("maf.local.resources.operation.stop", this, "stopOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.undo", this, "undoOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.redo", this, "redoOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.sizeUndoStack", this, "undoStackSizeSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.currentRunning", this, "currentOperationSignal()")
    mafRegisterLocalSignal("maf.local.resources.operation.executionPool", this, "executionPoolSignal()")

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.operation.start", this, "startOperation(const QString)")
    mafRegisterLocalCallback("maf.local.resources.operation.setParameters", this, "setOperationParameters(QVariantList)")
    mafRegisterLocalCallback("maf.local.resources.operation.execute", this, "executeOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.executeWithParameters", this, "executeWithParameters(QVariantList)")
    mafRegisterLocalCallback("maf.local.resources.operation.stop", this, "stopOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.undo", this, "undoOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.redo", this, "redoOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.sizeUndoStack", this, "undoStackSize()")
    mafRegisterLocalCallback("maf.local.resources.operation.currentRunning", this, "currentOperation()")
    mafRegisterLocalCallback("maf.local.resources.operation.executionPool", this, "executionPool()")
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

    //request of the selected vme
    mafObjectBase *selectedObj = NULL;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, selectedObj);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);
    mafResource *resource = qobject_cast<mafResource *>(selectedObj);
    
    // Create the instance of the new operation to execute.
    m_CurrentOperation = (mafOperation *)mafNEWFromString(operation);

    if(m_CurrentOperation == NULL) {
        qWarning() << mafTr("Operation type '%1' not created. It needs to be register into the mafObjectFactory!!").arg(operation);
        return;
    }

    m_CurrentOperation->initialize();
    // Assign as input the current selected VME.
    m_CurrentOperation->setInput(resource);

    // Notify the observers that the new operation has started.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase*, m_CurrentOperation));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.started", mafEventTypeLocal, &argList);
}

void mafOperationManager::executeOperation() {
    if ( m_CurrentOperation != NULL ) {
        if (m_LastUndoneOperation != NULL) {
            QList<mafOperation*>::Iterator i;
            for ( i = m_UndoStack.begin() ; i != m_UndoStack.end(); ++i ) {
                if (m_LastUndoneOperation == *i) {
                    break;
                }
            }
            cleanUndoStack(i, m_UndoStack.end());
            m_LastUndoneOperation = NULL;
        }
        
        // put it into the undo stack.
        if ( m_CurrentOperation->canUnDo() ) {
            m_UndoStack << m_CurrentOperation;
        } else {
            cleanUndoStack(m_UndoStack.begin(), m_UndoStack.end());
        }

        qDebug() << "creating worker for operation " << m_CurrentOperation->objectName();
        // Create a resource worker and pass to it the resource to be execute in a separate thread.
        mafOperationWorker *worker = new mafOperationWorker(m_CurrentOperation);
        // become observer wo be notified when the work is done.
        connect(worker, SIGNAL(workDone()), this, SLOT(executionEnded()));
        connect(worker, SIGNAL(workAborted()), this, SLOT(stopOperation()));
        // Put the worker into the pool.
        m_ExecutionPool << worker;
        // Start the work.
        qDebug() << "Starting worker...";
        worker->start();
    } else {
        qWarning() << mafTr("No Current operation!! startOperation required before executeOperation.");
    }
}

mafOperationWorker *mafOperationManager::removeWorkerFromPool(QObject *obj) {
    mafOperationWorker *worker = qobject_cast<mafResources::mafOperationWorker *>(obj);

    QMutableListIterator<QPointer<QThread> > i(m_ExecutionPool);
    while (i.hasNext()) {
        QThread *w = i.next();
        if (w == worker) {
            if (w->wait(100)) {
                i.remove();
            }
        }
    }

    return worker;
}

void mafOperationManager::executionEnded() {
    // Remove the resource worker from the list.
    mafOperationWorker *worker = removeWorkerFromPool(QObject::sender());

    mafOperation *op = worker->operation();
    REQUIRE(op != NULL);

    QString name = op->objectName();

    if ( !op->canUnDo() ) {
        mafDEL(op);
    } else {
        int idx = m_UndoStack.indexOf(op);
        if ( idx == -1 ) {
            mafDEL(op);
        }
    }
    delete worker;

    qDebug() << "Sending operation.executed for operation " << name;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.executed");
}

void mafOperationManager::stopOperation() {
    // Remove the resource worker from the list.

  mafOperationWorker *worker = removeWorkerFromPool(QObject::sender());

    if ( worker == NULL ) {
        // Operation not executing => Simply cancelled by the user.
        // m_CurrentOperation should be deleted.
        m_CurrentOperation->terminate();
        mafDEL(m_CurrentOperation);
        return;
    }

    mafOperation *op = worker->operation();
    REQUIRE(op != NULL);

    int idx = m_UndoStack.indexOf(op);
    if ( idx != -1 ) {
        m_UndoStack.removeAt(idx);
    }

    mafDEL(op);

    // Current operation ended => reset the pointer.
    m_CurrentOperation = m_UndoStack.size() > 0 ? m_UndoStack.last() : NULL;
    delete worker;
}

void mafOperationManager::undoOperation() {
    if ( m_CurrentOperation == NULL ) {
        // No operations in the undo stack.
        return;
    }
    
    if ( m_CurrentOperation->isRunning() ) {
        // Should never enter here.
        qWarning() << mafTr("Cannot perform the UnDo, because operation %1 is still running in background!").arg(m_CurrentOperation->objectName());
        return;
    }

    mafUndoStackCommand *usc = new mafUndoStackCommand(m_CurrentOperation, "unDo()");
    usc->execute();
    mafDEL(usc);

    // Set the operation to undo.
    QList<mafOperation*>::Iterator i;
    m_LastUndoneOperation = m_CurrentOperation;

    // At lease one undo has been done. Find which is the next op to undo.
    for ( i = m_UndoStack.begin() ; i != m_UndoStack.end(); ++i ) {
        if ( *i == m_LastUndoneOperation ) {
            if ( i == m_UndoStack.begin() ) {
                m_CurrentOperation = NULL;
            } else {
                m_CurrentOperation = *(--i);
            }
            break;
        }
    }
}

void mafOperationManager::redoOperation() {
    if ( m_LastUndoneOperation == NULL ) {
        return;
    }

    mafUndoStackCommand *usc = new mafUndoStackCommand(m_LastUndoneOperation, "reDo()");
    usc->execute();
    mafDEL(usc);
    
    m_CurrentOperation = m_LastUndoneOperation;
    
    QList<mafOperation*>::Iterator i;
    for ( i = m_UndoStack.begin() ; i != m_UndoStack.end(); ++i ) {
        if ( *i == m_CurrentOperation ) {
            if ( i == m_UndoStack.end() ) {
                m_LastUndoneOperation = NULL;
            } else {
                m_LastUndoneOperation = *(++i);
            }
            break;
        }
    }
}

void mafOperationManager::cleanUndoStack(QList<mafOperation*>::Iterator start, QList<mafOperation*>::Iterator end) {
    QList<mafOperation*>::Iterator iter = start;
    mafOperation *op;
    while(iter != end) {
        op = *iter;
        if ( !op->isRunning() ) {
            mafDEL(op);
        }
        ++iter;
    }
    m_UndoStack.erase(start, end);
}

void mafOperationManager::flushUndoStack() {
    mafOperation *op;
    foreach(op, m_UndoStack) {
        mafDEL(op);
    }
    m_UndoStack.clear();
}
