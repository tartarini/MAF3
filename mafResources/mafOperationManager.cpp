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


using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafOperationManager* mafOperationManager::instance() {
    // Create the instance of the VME manager.
    static mafOperationManager instanceOperationManager;
    return &instanceOperationManager;
}

void mafOperationManager::shutdown() {
}

mafOperationManager::mafOperationManager(const mafString code_location) : mafObjectBase(code_location), m_LastExecutedOperation(NULL), m_CurrentOperation(NULL) {
    initializeConnections();
}

mafOperationManager::~mafOperationManager() {
    clearUndoStack();
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
    provider->createNewId("maf.local.resources.operation.clearUndoStack");
    provider->createNewId("maf.local.resources.operation.sizeUndoStack");
    provider->createNewId("maf.local.resources.operation.currentRunning");
    provider->createNewId("maf.local.resources.operation.lastExecuted");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.operation.start", this, "startOperationSignal(const mafString)");
    mafRegisterLocalSignal("maf.local.resources.operation.started", this, "operationDidStart(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("maf.local.resources.operation.setParameters", this, "setOperationParametersSignal(mafList<mafVariant>)");
    mafRegisterLocalSignal("maf.local.resources.operation.execute", this, "executeOperationSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.executeWithParameters", this, "executeWithParametersSignal(mafList<mafVariant>)");
    mafRegisterLocalSignal("maf.local.resources.operation.stop", this, "stopOperationSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.undo", this, "undoOperationSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.redo", this, "redoOperationSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.clearUndoStack", this, "clearUndoStackSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.sizeUndoStack", this, "undoStackSizeSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.currentRunning", this, "currentOperationSignal()");
    mafRegisterLocalSignal("maf.local.resources.operation.lastExecuted", this, "lastExecutedOperationSignal()");

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.operation.start", this, "startOperation(const mafString)");
    mafRegisterLocalCallback("maf.local.resources.operation.setParameters", this, "setOperationParameters(mafList<mafVariant>)");
    mafRegisterLocalCallback("maf.local.resources.operation.execute", this, "executeOperation()");
    mafRegisterLocalCallback("maf.local.resources.operation.executeWithParameters", this, "executeWithParameters(mafList<mafVariant>)");
    mafRegisterLocalCallback("maf.local.resources.operation.stop", this, "stopOperation()");
    mafRegisterLocalCallback("maf.local.resources.operation.undo", this, "undoOperation()");
    mafRegisterLocalCallback("maf.local.resources.operation.redo", this, "redoOperation()");
    mafRegisterLocalCallback("maf.local.resources.operation.clearUndoStack", this, "clearUndoStack()");
    mafRegisterLocalCallback("maf.local.resources.operation.sizeUndoStack", this, "undoStackSize()");
    mafRegisterLocalCallback("maf.local.resources.operation.currentRunning", this, "currentOperation()");
    mafRegisterLocalCallback("maf.local.resources.operation.lastExecuted", this, "lastExecutedOperation()");

    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)");
}


void mafOperationManager::vmeSelect(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafResources::mafVME*>(obj);
    if(vme && vme != m_SelectedVME) {
        // VME has been selected.
        m_SelectedVME = vme;

//        const QMetaObject* metaVme = vme->metaObject();
//
//        m_OperationAcceptCurentVMEMap.clear();
//        foreach(mafResource *v, m_OperationsList) {
//            const QMetaObject* metaOp = v->metaObject();
//            //need a check on all the operations for accept objects
//            mafStringList binding_class_list;
//            binding_class_list = mafResourcesRegistration::acceptObject(v);
//            bool isAccepted = binding_class_list.contains(metaVme->className());
//            // instead of name , put struct with label
//            m_OperationAcceptCurentVMEMap.insert(metaOp->className(), isAccepted);
//        }
    }
}

void mafOperationManager::executeWithParameters(mafList<mafVariant> op_with_parameters) {
    REQUIRE(op_with_parameters.count() == 2);
    //parameters contains as first argument the operation name
    mafString op_to_run = op_with_parameters.at(0).toString();
    REQUIRE(!op_to_run.isEmpty());

    this->startOperation(op_to_run);
    //parameters contains as second argument a list of mafVariant which are passed to the operation
    m_CurrentOperation->setParameters(op_with_parameters.at(1).toList());
    this->executeOperation();
}

void mafOperationManager::setOperationParameters(mafList<mafVariant> parameters) {
    REQUIRE(m_CurrentOperation);
    m_CurrentOperation->setParameters(parameters);
}

void mafOperationManager::startOperation(const mafString operation) {
    REQUIRE(!operation.isEmpty());
    REQUIRE(m_CurrentOperation == NULL); //require also that there isn't running operation

    // Create the instance of the new operation to execute.
    m_CurrentOperation = (mafOperation *)mafNEWFromString(operation);

    if(m_CurrentOperation == NULL) {
        mafMsgWarning() << mafTr("Operation type '%1' not created. It needs to be register into the mafObjectFactory!!").arg(operation);
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
    if(m_CurrentOperation != NULL) {
        m_CurrentOperation->execute();
        // check if operation canUndo
        if(!m_CurrentOperation->canUnDo()) {
            clearUndoStack();
            m_LastExecutedOperation = NULL;
            mafDEL(m_CurrentOperation);
            return;
        }

        if(m_CurrentOperation->isRunning()) {
            //undo stack insertion
            mafLinkedList<mafOperation*>::Iterator i;
            if(m_LastExecutedOperation == NULL) {
                clearUndoStack();
            } else {
                bool found = false;
                for(i = m_UndoStack.begin() ; i != m_UndoStack.end(); i++) {
                    if(found) {
                        mafDEL(*i);
                    }
                    if(*i == m_LastExecutedOperation) {
                        found = true;
                    }
                }

                while(m_UndoStack.last() == NULL) {
                    m_UndoStack.pop_back();
                }
            }


            m_LastExecutedOperation = m_CurrentOperation;
            m_UndoStack.push_back(m_LastExecutedOperation);
        }
        m_CurrentOperation = NULL;
    }
}

void mafOperationManager::stopOperation() {
    REQUIRE(m_CurrentOperation != NULL);

    m_CurrentOperation->terminate();
    if(m_CurrentOperation->canUnDo()) {
        m_UndoStack.pop_back();
    }
    mafDEL(m_CurrentOperation);
}

void mafOperationManager::undoOperation() {
    mafUndoStackCommand usc(m_LastExecutedOperation, "unDo");
    usc.execute();
    //m_LastExecutedOperation->unDo();
    mafLinkedList<mafOperation*>::Iterator i;
    for(i = m_UndoStack.begin() ; i != m_UndoStack.end(); i++) {
        if(*i == m_LastExecutedOperation) {
            if(i == m_UndoStack.begin()) {
                m_LastExecutedOperation = NULL;
            } else {
                m_LastExecutedOperation = *(--i);
            }
            break;
        }
    }
}

void mafOperationManager::redoOperation() {
    mafLinkedList<mafOperation*>::Iterator i;
    for(i = m_UndoStack.begin() ; i != m_UndoStack.end(); i++) {
        if(m_LastExecutedOperation == NULL) { //keep separate from below condition only for undertanding better
            //means that with undo, user arrives at the beginning
            m_LastExecutedOperation = *(m_UndoStack.begin());
            mafUndoStackCommand usc(m_LastExecutedOperation, "reDo");
            usc.execute();
            //m_LastExecutedOperation->reDo();
            break;
        }
        if(*i == m_LastExecutedOperation) {
            if(++i == m_UndoStack.end()) {
                mafMsgWarning("%s", mafTr("Can't redo, undo stack finished").toAscii().data());
            } else {
                m_LastExecutedOperation = *i;
                mafUndoStackCommand usc(m_LastExecutedOperation, "reDo");
                usc.execute();
                //m_LastExecutedOperation->reDo();
                break;
            }
        }
    }
}

const mafCore::mafObjectBase *mafOperationManager::lastExecutedOperation() const {
    return (mafCore::mafObjectBase *)m_LastExecutedOperation;
}

void mafOperationManager::clearUndoStack() {
    mafOperation *op;
    foreach(op, m_UndoStack) {
        mafDEL(op);
    }
    m_UndoStack.clear();
    m_LastExecutedOperation = NULL;
}
