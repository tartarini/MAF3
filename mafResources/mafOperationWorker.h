/*
 *  mafOperationWorker.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCEWORKER_H
#define MAFRESOURCEWORKER_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafOperation;

/// Worker class to execute an algorithm in a separate thread.
/**
Class name: mafOperationWorker
This class defines the class used to execute an algorithm in a separate thread.
@sa mafOperationManager
*/
class MAFRESOURCESSHARED_EXPORT mafOperationWorker : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafOperationWorker(const QString code_location = "");

    /// Object constructor.
    mafOperationWorker(mafOperation *res, const QString code_location = "");

    /// Return the owned resource.
    mafOperation *operation();

    /// Start the background execution.
    void doWork();

public slots:
    /// Abort the operation execution thread.
    void abortExecution();

signals:
    /// Signal emitted to notify that the work has been completed.
    void workDone();

    /// Signal emitted to notify that the worker has been aborted.
    void workAborted();

protected:
    /// Object destructor.
    /* virtual */ ~mafOperationWorker();

private:
    /// Initialize the operation's workflow executed by the state machine.
    void initializeWorkflow();

    QThread *m_ExecutionThread; ///< Thread on which will be executed the operation
    mafOperation *m_Operation;    ///< Operation to be executed on separate thread.
    QStateMachine m_StateMachine;   ///< State machine responsible of the operation execution workflow.
};

inline mafOperation *mafOperationWorker::operation() {
    return m_Operation;
}

typedef mafOperationWorker * mafResourceWorkerPointer;

} // namespace mafResources

Q_DECLARE_METATYPE(mafResources::mafResourceWorkerPointer)

#endif // MAFRESOURCEWORKER_H
