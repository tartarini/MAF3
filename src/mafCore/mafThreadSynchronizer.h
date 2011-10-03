/*
 *  mafThreadSynchronizer.h
 *  mafCore
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 03/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTHREADSYNCHRONIZER_H
#define MAFTHREADSYNCHRONIZER_H

#include "mafCore_global.h"
#include "mafCoreDefinitions.h"

namespace mafCore {

/**
Class name: mafThreadSynchronizer
Class with the capability of thread synchronizations.
It is a singleton.
*/
class MAFCORESHARED_EXPORT mafThreadSynchronizer {
public:
    /// Return the instance of the singleton object.
    static mafThreadSynchronizer *instance(void);

    /// Shutdown the thread synchronizer.
    void shutdown();

    /// Lock the the instance.
    void lock(void);
    
    /// Unlock the the instance.
    void unlock(void);
    
    /// Wait a mutex condition.
    void wait(void);
    
    /// Use the wake condition.
    void wake(void);

protected:
    /// Object constructor.
     mafThreadSynchronizer(void);
    /// Object destructor.
    ~mafThreadSynchronizer(void);

private:
    static mafThreadSynchronizer *m_Instance; /// singleton object

    QMutex         *m_Mutex; ///< mutex for lock, unlock the singleton
    QWaitCondition *m_Condition; ///< wait condition
};

} // namespace mafCore

#endif
