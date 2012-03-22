/*
 *  mafScriptEditorPythonModuleManager.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTEDITORPYTHONMODULEMANAGER_H
#define MAFSCRIPTEDITORPYTHONMODULEMANAGER_H

#include "mafScriptInterpreter_global.h"
#include "mafScriptEditorPython.h"


namespace mafScriptInterpreter {

/**
    Class Name: mafScriptEditorPythonModuleManager
    Manages Python Interpreter.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPythonModuleManager : public QObject {
    /// pointer to the initializer function
    typedef int (*mafScriptEditorModuleInitializer)(void);

public:
    /// Return the singleton instance.
    static mafScriptEditorPythonModuleManager* instance(void);

    /// Initialize interpreters.
    void initialize(mafScriptEditorPython *interpreter);

    /// Register initializer function.
    void registerInitializer(mafScriptEditorModuleInitializer initializer);

    /// Register commands.
    void registerCommand(QString command);

protected:
    /// Object Constructor.
    mafScriptEditorPythonModuleManager(void) {}

    /// Object Destructor.
    ~mafScriptEditorPythonModuleManager(void) {}

private:
    QList<mafScriptEditorModuleInitializer> m_Initializers; ///< list of initializer functions
    QList<QString> m_Commands; /// list of commands.

    static mafScriptEditorPythonModuleManager *m_Instance; /// singleton instance.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafScriptEditorPythonModuleManager::registerInitializer(mafScriptEditorModuleInitializer initializer) {
    m_Initializers << initializer;
}

inline void mafScriptEditorPythonModuleManager::registerCommand(QString command) {
    m_Commands << command;
}

} // end namespace

#endif // MAFSCRIPTEDITORPYTHONMODULEMANAGER_H
