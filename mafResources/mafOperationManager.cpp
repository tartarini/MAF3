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
    provider->createNewId("RESOURCES_OPERATION_START");
    provider->createNewId("RESOURCES_OPERATION_SET_PARAMETERS");
    provider->createNewId("RESOURCES_OPERATION_EXECUTE");
    provider->createNewId("RESOURCES_OPERATION_EXECUTE_WITH_PARAMETERS");
    provider->createNewId("RESOURCES_OPERATION_STOP");
    provider->createNewId("RESOURCES_OPERATION_UNDO");
    provider->createNewId("RESOURCES_OPERATION_REDO");
    provider->createNewId("RESOURCES_OPERATION_CLEAR_UNDOSTACK");
    provider->createNewId("RESOURCES_OPERATION_SIZE_UNDOSTACK");
    provider->createNewId("RESOURCES_OPERATION_RETURN_CURRENT_OPERATION_RUNNING");
    provider->createNewId("RESOURCES_OPERATION_RETURN_LAST_EXECUTED_OPERATION");

    // Register API signals.
    mafRegisterLocalSignal("RESOURCES_OPERATION_START", this, "startOperationSignal(const mafString)");
    mafRegisterLocalSignal("RESOURCES_OPERATION_SET_PARAMETERS", this, "setOperationParametersSignal(mafList<mafVariant>)");
    mafRegisterLocalSignal("RESOURCES_OPERATION_EXECUTE", this, "executeOperationSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_EXECUTE_WITH_PARAMETERS", this, "executeWithParametersSignal(mafList<mafVariant>)");
    mafRegisterLocalSignal("RESOURCES_OPERATION_STOP", this, "stopOperationSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_UNDO", this, "undoOperationSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_REDO", this, "redoOperationSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_CLEAR_UNDOSTACK", this, "clearUndoStackSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_SIZE_UNDOSTACK", this, "undoStackSizeSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_RETURN_CURRENT_OPERATION_RUNNING", this, "currentOperationSignal()");
    mafRegisterLocalSignal("RESOURCES_OPERATION_RETURN_LAST_EXECUTED_OPERATION", this, "lastExecutedOperationSignal()");


    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("RESOURCES_OPERATION_START", this, "startOperation(const mafString)");
    mafRegisterLocalCallback("RESOURCES_OPERATION_SET_PARAMETERS", this, "setOperationParameters(mafList<mafVariant>)");
    mafRegisterLocalCallback("RESOURCES_OPERATION_EXECUTE", this, "executeOperation()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_EXECUTE_WITH_PARAMETERS", this, "executeWithParameters(mafList<mafVariant>)");
    mafRegisterLocalCallback("RESOURCES_OPERATION_STOP", this, "stopOperation()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_UNDO", this, "undoOperation()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_REDO", this, "redoOperation()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_CLEAR_UNDOSTACK", this, "clearUndoStack()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_SIZE_UNDOSTACK", this, "undoStackSize()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_RETURN_CURRENT_OPERATION_RUNNING", this, "currentOperation()");
    mafRegisterLocalCallback("RESOURCES_OPERATION_RETURN_LAST_EXECUTED_OPERATION", this, "lastExecutedOperation()");
}


void mafOperationManager::vmeSelected(mafVME *vme) {
    if(vme) {
        // VME has been selected.
        m_SelectedVME = vme;

        const QMetaObject* metaVme = vme->metaObject();

        m_OperationAcceptCurentVMEMap.clear();
        foreach(mafResource *v, m_OperationsList) {
            const QMetaObject* metaOp = v->metaObject();
            //need a check on all the operations for accept objects
            mafStringList binding_class_list;
            binding_class_list = mafResourcesRegistration::acceptObject(v);
            bool isAccepted = binding_class_list.contains(metaVme->className());
            // instead of name , put struct with label
            m_OperationAcceptCurentVMEMap.insert(metaOp->className(), isAccepted);
        }
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

    m_CurrentOperation = (mafOperation *)mafNEWFromString(operation);
    bool result = m_CurrentOperation->initialize();

    ENSURE(result);
}

void mafOperationManager::executeOperation() {
    if(m_CurrentOperation != NULL) {
        bool result = m_CurrentOperation->execute();
        // check if operation canUndo
        if(!m_CurrentOperation->canUnDo()) {
            clearUndoStack();
            m_LastExecutedOperation = NULL;
            mafDEL(m_CurrentOperation);
            return;
        }

        if(result) {
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
    if(m_CurrentOperation != NULL) {
        m_CurrentOperation->terminate();
    }

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
