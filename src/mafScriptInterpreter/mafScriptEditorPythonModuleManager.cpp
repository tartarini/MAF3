/*
 *  mafScriptEditorPythonModuleManager.cpp
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafScriptEditorPythonModuleManager.h"

using namespace mafScriptInterpreter;

MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPythonModuleManager *mafScriptEditorPythonModuleManager::m_Instance = NULL;

mafScriptEditorPythonModuleManager* mafScriptEditorPythonModuleManager::instance(void) {
    if(!m_Instance) {
        m_Instance = new mafScriptEditorPythonModuleManager;
    }

    return m_Instance;
}

void mafScriptEditorPythonModuleManager::initialize(mafScriptEditorPython *interpreter) {
    int stat;
    interpreter->blockThreads();
    Q_FOREACH(mafScriptEditorModuleInitializer initializer, m_Initializers)
        initializer();

    interpreter->allowThreads();
    Q_FOREACH(QString command, m_Commands) {
        interpreter->interpret(command, &stat);
    }
}
