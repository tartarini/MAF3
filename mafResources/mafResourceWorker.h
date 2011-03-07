/*
 *  mafResourceWorker.h
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
class mafResource;

/// Worker class to execute an algorithm in a separate thread.
/**
Class name: mafResourceWorker
This class defines the class used to execute an algorithm in a separate thread.
@sa mafOperationManager
*/
class MAFRESOURCESSHARED_EXPORT mafResourceWorker : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafResourceWorker(const QString code_location = "");

    /// Object constructor.
    mafResourceWorker(mafResource *res, const QString code_location = "");

    /// Return the owned resource.
    mafResource *resource();

    /// Start the background execution.
    void doWork();

signals:
    /// Signal emitted to abort the resource execution.
    void abortFlag();

    /// Signal emitted to notify that the work has been completed.
    void workDone();

protected:
    /// Object destructor.
    /* virtual */ ~mafResourceWorker();

private:
    QThread *m_ExecutionThread; ///< Thread on which will be executed the resource
    mafResource *m_Resource;       ///< Resource to be executed on separate thread.
};

inline mafResource *mafResourceWorker::resource() {
    return m_Resource;
}

typedef mafResourceWorker * mafResourceWorkerPointer;

} // namespace mafResources

Q_DECLARE_METATYPE(mafResources::mafResourceWorkerPointer)

#endif // MAFRESOURCEWORKER_H
