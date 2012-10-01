/*
 *  mafViewScriptInterpreterECMAScript.cpp
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 27/03/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewScriptInterpreterECMAScript.h"
#include "mafInterpreterConsole.h"
#include "mafScriptEditorECMAScript.h"
#include "mafScriptEditorPool.h"

using namespace mafScriptInterpreter;

mafViewScriptInterpreterECMAScript::mafViewScriptInterpreterECMAScript(const QString code_location) : mafViewScriptInterpreter(code_location) {
}

mafViewScriptInterpreterECMAScript::~mafViewScriptInterpreterECMAScript() {
}

bool mafViewScriptInterpreterECMAScript::initialize() {
    if(Superclass::initialize()) {
        mafScriptEditor *interpreter = mafScriptEditorPool::instance()->console("ecma");
        ((mafInterpreterConsole*)m_RenderWidget)->registerInterpreter(interpreter);
    
        return interpreter!=NULL;
    }
    
    return false;
}