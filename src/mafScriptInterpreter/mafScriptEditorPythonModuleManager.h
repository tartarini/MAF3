/*
 *  mafScriptEditorPythonModuleManager.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
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
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPythonModuleManager : public QObject
{
    /// pointer to the initializer function
    typedef int (*mafScriptEditorModuleInitializer)(void);

public:
    /// Return the singleton instance.
    static mafScriptEditorPythonModuleManager* instance(void) {
	if(!m_instance)
	    m_instance = new mafScriptEditorPythonModuleManager;

	return m_instance;
    }

    /// Initialize interpreters.
    void initialize(mafScriptEditorPython *interpreter) {
	int stat;
    interpreter->blockThreads();
	foreach(mafScriptEditorModuleInitializer initializer, initializers)
	    initializer();

    interpreter->allowThreads();
	foreach(QString command, commands) 
	    interpreter->interpret(command, &stat);
    }

    /// Register initializer function.
    void registerInitializer(mafScriptEditorModuleInitializer initializer) {
	initializers << initializer;
    }

    /// Register commands.
    void registerCommand(QString command) {
	commands << command;
    }

protected:
    /// Object Constructor.
    mafScriptEditorPythonModuleManager(void) {}
    /// Object Destructor.
    ~mafScriptEditorPythonModuleManager(void) {}

private:
    QList<mafScriptEditorModuleInitializer> initializers; ///< list of initializer functions
    QList<QString> commands; /// list of commands.

    static mafScriptEditorPythonModuleManager *m_instance; /// singleton instance.
};

} // end namespace
    
#endif // MAFSCRIPTEDITORPYTHONMODULEMANAGER_H
