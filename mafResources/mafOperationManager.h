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
class mafResourceWorker;

/// This class provides the manager class for MAF3 operations.
/**
Class name: mafOperationManager
The mafOperationManager manage the creation and execution of operations. Operation are started in a separate thread
and according to the user's specification at GUI level, the operation can execute in background (asynchronous execution)
or no (synchronous execution). In the latter case the operation is always executed in a separate thread but the operation
manager waits that the thread is finished.
The manager defines these Topics:
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

signals:
    /// Signal connected to the startOperation private slot.
    void startOperationSignal(const QString operation);

    /// Signal used notify that the operation has been started.
    /**
        @param operation Instance of the started operation.
    */
    void operationDidStart(mafCore::mafObjectBase *operation);

    /// Signal connected to the setOperationParameters slot.
    void setOperationParametersSignal(QVariantList parameters);

    /// Signal connected to the executeOperation slot.
    void executeOperationSignal();

    /// Signal connected with executeWithParameters slot.
    void executeWithParametersSignal(QVariantList parameters);

    /// Signal connected with stopOperation slot.
    void stopOperationSignal();

    /// Signal connecte dwith undoOperation slot.
    void undoOperationSignal();

    /// Signal connected with redoOperation slot.
    void redoOperationSignal();

    /// Signal connected with updateUndoStack slot.
    void updateUndoStackSignal(mafCore::mafObjectBase *op);

    /// Signal ocnnected with undoStackSize slot.
    int undoStackSizeSignal() const;

    /// Signal connected with currentOperation slot.
    const mafCore::mafObjectBase *currentOperationSignal() const;

private slots:
    /// Start operation and set that operation as current one
    /**
        This method allows to create and start a given operation's type. After its initialization an operationDidStart signal is
        emitted, so that the requestor can be informed about the operation start status. The operation that has to be started
        has to be registered with the mafObjectFactory.
        @param operation Type of operation to create and start.
    */
    void startOperation(const QString operation);

    /// Set the parameters to the current started operation.
    /**
        @param parameters List of parameters to be asigned to the current operation.
    */
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

    /// Execute the operation with given parameters.
    /**
        Initialize the given operation (first element of the list), pass to it the given (optionals) parameters as second element
        of the list and start the execution of the operation.
        @param op_with_parameters List containing as first element the operation's type to be executed. The rest of list elements are operation's parameters.
    */
    void executeWithParameters(QVariantList op_with_parameters);

    /// Called when a vme has been selected.
    void vmeSelect(mafCore::mafObjectBase *vme);

    /// Stop current operation
    void stopOperation();

    /// Handle the undo stack removing last operation effect
    void undoOperation();

    /// Reapply operation results
    void redoOperation();

    /// Clear undo stack destroying all allocated operations before the execution of not-undoable operation passed as argument.
    /**
        This method allows to clear all the operations present into the stack before that executed and can't undo.
        The operation that doesn't support the undo is passed as argument. If NULL is passed, all the undo stack is cleared.
        @param no_undo_op Operation that doesn't support the undo.
    */
    void updateUndoStack(mafCore::mafObjectBase *no_undo_op);

    /// Return the size of the stack undo in terms of operation numbers
    int undoStackSize() const;

    /// Return current operation
    const mafCore::mafObjectBase *currentOperation() const;

protected:
    /// Object destructor
    /*virtual*/ ~mafOperationManager();

private:
    /// Object constructor.
    mafOperationManager(const QString code_location = "");

    /// Create Topics and connect signals with slots.
    void initializeConnections();

    /// Remove all the objects present into the undo stack and free all the allocated memory.
    void flushUndoStack();

    /// Remove the passed object from the execution pool.
    /**
        This method check if the passed object is a mafResourceWorker and eventually remove it from the execution pool.
        The method return to the caller a casted pointer to mafResourceWorker removed from the pool. NULL is returned
        if the possed argument is not a mafResourceWorker.
        @param obj Pointer to the mafResourceWorker.
    */
    mafResourceWorker *removeWorkerFromPool(QObject *obj);

    QList<mafOperation *> m_UndoStack;    ///< Undo stack which is a linked list of operations
    mafOperation *m_CurrentOperation;       ///< Current operation handled by th manager
    QVector<mafResourceWorker *> m_ExecutionPool; ///< Pool of running operations.
    mafVME *m_SelectedVME;                      ///< Vme that is currently selected
    mafCore::mafId m_ExecWithParameters; ///< Id associated with the EXECUTE_WITH_PARAMETERS event.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline const mafCore::mafObjectBase *mafOperationManager::currentOperation() const{
    return (mafCore::mafObjectBase *)m_CurrentOperation;
}

inline int mafOperationManager::undoStackSize() const {
    return m_UndoStack.size();
}


} // namespace mafResources


#endif // MAFOPERATIONMANAGER_H
