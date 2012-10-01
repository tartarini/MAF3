/*
 *  mafOperationManager.h
 *  mafResources
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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
class mafOperationWorker;

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
- maf.local.resources.operation.executed notify that the operation terminated its execution and return an output as VariantHash.
- maf.local.resources.operation.executeWithParameters start the execution of operation.
- maf.local.resources.operation.stop terminate the execution of the operation.
- maf.local.resources.operation.undo allow to navigate in undo stack in order to restore data before current operation.
- maf.local.resources.operation.redo allow to navigate in undo stack in order apply the next operation.
- maf.local.resources.operation.clearundoStack clear the undo stack.
- maf.local.resources.operation.sizeUndoStack retrieve number of elements of undo stack.
- maf.local.resources.operation.currentRunning retrieve current operation.
- maf.local.resources.operation.executionPool Return the execution pool containing the running operations.
- maf.local.resources.operation.canSelectVME Return a boolean checking if the passed vme can be selected.

@sa mafOperation mafOperationWorker
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

Q_SIGNALS:
    /// Signal connected to the startOperation private slot.
    void startOperationSignal(const QString operation);

    /// Signal used notify that the operation has been started.
    /**
        @param operation Instance of the started operation.
    */
    void operationDidStart(mafCore::mafObjectBase *operation);

    /// Signal connected to the setOperationParameters slot.
    void setOperationParametersSignal(const QVariantMap &parameters);

    /// Signal connected to the executeOperation slot.
    void executeOperationSignal();

    /// Signal emitted to notify that the operation ended its execution and the associated thread has been deleted.
    void executedOperationSignal(QVariantMap outputMap);

    /// Signal connected with executeWithParameters slot.
    void executeWithParametersSignal(QVariantList parameters);

    /// Signal connected with stopOperation slot.
    void stopOperationSignal();

    /// Signal connecte dwith undoOperation slot.
    void undoOperationSignal();

    /// Signal connected with redoOperation slot.
    void redoOperationSignal();

    /// Signal ocnnected with undoStackSize slot.
    int undoStackSizeSignal() const;

    /// Signal connected with currentOperation slot.
    const mafCore::mafObjectBase *currentOperationSignal() const;

    /// Signal connected with the executionPool slot.
    const mafExecutionPool *executionPoolSignal();
    
    /// Signal connected to canSelectVME slot.
    bool canSelectVMESignal(mafCore::mafObjectBase *vme);

private Q_SLOTS:
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
    void setOperationParameters(const QVariantMap &parameters);

    /// Execute current operation.
    /**
        This method allows to execute the operation in a separat thread, by instantiating the mafOperationWorker and
        giving to it the operation to execute. The worker that is put into the execution pool of the operation manager.
    */
    void executeOperation();

    /// Called when the operation has terminated its execution.
    /**
        All the operation should Q_EMIT the signal executionEnded at the end of the execution. In this way the operation
        manager is notified and can finalize the operation execution lifetime by placing the operation into the undo stack
        if it support the undo or it is deleted.
    */
    void executionEnded();

    /// Execute the operation with given parameters.
    /**
        Initialize the given operation (first element of the list), pass to it the given (optionals) parameters as second element
        of the list and start the execution of the operation.
        @param op_with_parameters List containing as first element the operation's type to be executed. The rest of list elements are operation's parameters.
    */
    void executeWithParameters(QVariantList op_with_parameters);

    /// Stop current operation
    void stopOperation();

    /// Handle the undo stack removing last operation effect
    void undoOperation();

    /// Reapply operation results
    void redoOperation();

    /// Clear undo stack destroying all allocated operations before the execution of not-undoable operation.
    /**
        This method allows to clear all the operations present into the stack before that executed and can't undo.
    */
    void cleanUndoStack(QList<mafOperation*>::Iterator start, QList<mafOperation*>::Iterator end);

    /// Return the size of the stack undo in terms of operation numbers
    int undoStackSize() const;

    /// Return current operation
    const mafCore::mafObjectBase *currentOperation() const;

    /// Return the execution pool containing the runngin operations in background.
    const mafExecutionPool *executionPool() const;

    /// return if the vme can be selected or not (checking if a non threadable operation is running or if the vme is not locked).
    bool canSelectVME(mafCore::mafObjectBase *vme);
    
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
        This method check if the passed object is a mafOperationWorker and eventually remove it from the execution pool.
        The method return to the caller a casted pointer to mafOperationWorker removed from the pool. NULL is returned
        if the possed argument is not a mafOperationWorker.
        @param obj Pointer to the mafOperationWorker.
    */
    mafOperationWorker *removeWorkerFromPool(QObject *obj);

    mutable QMutex m_Mutex;
    QList<mafOperation *> m_UndoStack;    ///< Undo stack which is a linked list of operations
    mafOperation *m_CurrentOperation;       ///< Current operation handled by th manager
    mafOperation *m_LastUndoneOperation;       ///< Last undone operation 

    mafExecutionPool m_ExecutionPool; ///< Pool of running operations.
    mafCore::mafId m_ExecWithParameters; ///< Id associated with the EXECUTE_WITH_PARAMETERS event.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline const mafCore::mafObjectBase *mafOperationManager::currentOperation() const {
    return (mafCore::mafObjectBase *)m_CurrentOperation;
}

inline int mafOperationManager::undoStackSize() const {
    return m_UndoStack.size();
}

inline const mafExecutionPool *mafOperationManager::executionPool() const {
    return (const mafExecutionPool*)&m_ExecutionPool;
}

} // namespace mafResources


#endif // MAFOPERATIONMANAGER_H
