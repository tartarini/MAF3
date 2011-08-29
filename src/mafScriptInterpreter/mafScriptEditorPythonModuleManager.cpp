/*
 *  mafScriptEditorPythonModuleManager.cpp
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafScriptEditorPythonModuleManager.h"

using namespace mafScriptInterpreter;

MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPythonModuleManager *mafScriptEditorPythonModuleManager::m_instance = NULL;

mafScriptEditorPythonModuleManager* mafScriptEditorPythonModuleManager::instance(void) {
    if(!m_instance) {
        m_instance = new mafScriptEditorPythonModuleManager;
    }

    return m_instance;
}

void mafScriptEditorPythonModuleManager::initialize(mafScriptEditorPython *interpreter) {
    int stat;
    interpreter->blockThreads();
    foreach(mafScriptEditorModuleInitializer initializer, initializers)
        initializer();

    interpreter->allowThreads();
    foreach(QString command, commands) 
        interpreter->interpret(command, &stat);
}
