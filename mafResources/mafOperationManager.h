/*
 *  mafOperationManager.h
 *  mafResources
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONMANAGER_H
#define MAFOPERATIONMANAGER_H

#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafOperation;
class mafVME;

/**
Class name: mafOperationManager
This class provides the manager class for MAF3 Operations.
The mafOperationManager defines the SET_PARAMETERS id that allows to assign parameter to the operaiton to run.
The manager defines these IDs:
- maf.local.resources.operation.start create the instance and initialize the operation.
- maf.local.resources.operation.started notify all the observer that the operation has started.
- maf.local.resources.operation.setParameters asign the parameters to the current started operation.
- maf.local.resources.operation.execute start the execution of operation.
- maf.local.resources.operation.executeWithParameters start the execution of operation.
- maf.local.resources.operation.stop terminate the execution of the operation.
- maf.local.resources.operation.undo allow to navigate in undo stack in order to restore data before current operation.
- maf.local.resources.operation.redo allow to navigate in undo stack in order apply the next operation.
- maf.local.resources.operation.clearundoStack clear the undo stack.
- maf.local.resources.operation.sizeUndoStack retrieve number of elements of undo stack.
- maf.local.resources.operation.currentRunning retrieve current operation.
- maf.local.resources.operation.lastExecuted retrieve last executed operation.
*/

class MAFRESOURCESSHARED_EXPORT mafOperationManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafOperationManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    typedef QMap<QString, bool> OperationAcceptVMEMap; /// typedef for creating a map which stores if the current vme has been accepted as input by the operation

signals:
    /// start operation and set that operation as current one
    void startOperationSignal(const QString operation);

    /// Notification signal for started operation.
    void operationDidStart(mafCore::mafObjectBase *operation);

    /// set the parameters to the current started operation
    void setOperationParametersSignal(QVariantList parameters);

    /// execute current operation
    void executeOperationSignal();

    /// Signal that allows to start an operation with some optionals parameters.
    void executeWithParametersSignal(QVariantList parameters);

    /// stop current operation
    void stopOperationSignal();

    /// handle the undo stack removing last operation effect
    void undoOperationSignal();

    /// reapply operation results
    void redoOperationSignal();

    /// clear undo stack destroying all allocated operations
    void clearUndoStackSignal();

    /// return the size of the stack undo in terms of operation numbers
    int undoStackSizeSignal() const;

    /// return current operation
    const mafCore::mafObjectBase *currentOperationSignal() const;

    /// return last executed operation
    const mafCore::mafObjectBase *lastExecutedOperationSignal() const;

private slots:
    /// start operation and set that operation as current one
    void startOperation(const QString operation);

    /// Set the parameters to the current started operation
    void setOperationParameters(QVariantList parameters);

    /// Execute current operation
    /**
        This method check the executeOnThread operation's property to decide if the execution has to be done in the main
        thread or in a separate one. If executeOnThread flag is "true" then the manager create an execution thread and move
        the operation on that thread, link the start thread signal with the execute operation's slot and start the thread.
    */
    void executeOperation();

    /// Called when the operation has terminated its execution.
    /**
        All the operation should emit the signal executionEnded at the end of the execution. In this way the operation
        manager is notified and can finalize the operation execution lifetime by placing the operation into the undo stack
        if it support the undo or it is deleted.
    */
    void operationExecuted();

    /// initialize the given operation (first element of the list), pass to it the given (optionals) parameters as second element of the list and start the execution of the operation.
    void executeWithParameters(QVariantList op_with_parameters);

    /// called when a vme has been selected.
    void vmeSelect(mafCore::mafObjectBase *vme);

    /// stop current operation
    void stopOperation();

    /// handle the undo stack removing last operation effect
    void undoOperation();

    /// reapply operation results
    void redoOperation();

    /// clear undo stack destroying all allocated operations
    void clearUndoStack();

    /// return the size of the stack undo in terms of operation numbers
    int undoStackSize() const;

    /// return current operation
    const mafCore::mafObjectBase *currentOperation() const;

    /// return last executed operation
    const mafCore::mafObjectBase *lastExecutedOperation() const;

protected:
    /// Object destructor
    /*virtual*/ ~mafOperationManager();

private:
    /// create IDs and connect signals with slots for View create, destroy, select and VME show.
    void initializeConnections();

    /// Object constructor.
    mafOperationManager(const QString code_location = "");

    mafOperation *m_LastExecutedOperation; ///< Last executed operation
    QLinkedList<mafOperation *> m_UndoStack; ///< Undo stack which is a linked list of operations
    mafOperation *m_CurrentOperation; ///< Current operation handled by th manager
//    mafResourceList m_OperationsList; ///< List of plugged operations.
//    OperationAcceptVMEMap m_OperationAcceptCurentVMEMap; /// Map which determines if the current vme can be input of the operations
    mafVME *m_SelectedVME; ///< Vme that is currently selected
    mafCore::mafId m_ExecWithParameters; ///< Id associated with the EXECUTE_WITH_PARAMETERS event.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline const mafCore::mafObjectBase *mafOperationManager::currentOperation() const{
    return (mafCore::mafObjectBase *)m_CurrentOperation;
}

inline int mafOperationManager::undoStackSize() const{
    return m_UndoStack.size();
}


} // namespace mafResources


#endif // MAFOPERATIONMANAGER_H
