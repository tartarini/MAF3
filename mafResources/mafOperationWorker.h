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
class MAFRESOURCESSHARED_EXPORT mafOperationWorker : public QThread {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    explicit mafOperationWorker();

    /// Object constructor.
    explicit mafOperationWorker(mafOperation *res);

    /// Return the owned resource.
    mafOperation *operation();

    /// Object destructor.
    /* virtual */ ~mafOperationWorker();

public slots:
    /// Abort the operation execution thread.
    void abortExecution();

signals:
    /// Signal emitted to notify that the work has been completed.
    void workDone();

    /// Signal emitted to notify that the worker has been aborted.
    void workAborted();

private:
    /// Initialize the operation's workflow executed by the state machine.
    void initializeWorkflow();

    /// Start the execution.
    void run();

    mafOperation *m_Operation;    ///< Operation to be executed on separate thread.
};

inline mafOperation *mafOperationWorker::operation() {
    return m_Operation;
}

typedef mafOperationWorker * mafResourceWorkerPointer;

} // namespace mafResources

Q_DECLARE_METATYPE(mafResources::mafResourceWorkerPointer)

#endif // MAFRESOURCEWORKER_H
