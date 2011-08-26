/*
 *  mafScriptEditorSynchronizer.h
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi on 08/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTINTERPRETER_H
#define MAFSCRIPTINTERPRETER_H

#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

/**
Class name: mafScriptEditorSynchronizer
Class with the capability of synchronizing editors when using multiple threads.
It is a singleton.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorSynchronizer {
public:
    /// Return the instance of the singleton object.
    static mafScriptEditorSynchronizer *instance(void);

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
     mafScriptEditorSynchronizer(void);
    /// Object destructior.
    ~mafScriptEditorSynchronizer(void);

private:
    static mafScriptEditorSynchronizer *m_instance; /// singleton object

    QMutex         *m_Mutex; ///< mutex for lock, unlock the singleton
    QWaitCondition *m_Condition; ///< wait condition
};

} // end namespace

#endif
