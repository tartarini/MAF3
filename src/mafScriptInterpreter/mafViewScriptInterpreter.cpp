/*
 *  mafViewScriptInterpreter.cpp
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 03/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewScriptInterpreter.h"
#include "mafInterpreterConsole.h"

using namespace mafResources;
using namespace mafScriptInterpreter;

mafViewScriptInterpreter::mafViewScriptInterpreter(const QString code_location) : mafView(code_location) {
    m_RenderWidget = new mafInterpreterConsole();
    m_RenderWidget->setObjectName("ScriptInterpreter");
    ((mafInterpreterConsole*)m_RenderWidget)->readSettings();
}

mafViewScriptInterpreter::~mafViewScriptInterpreter() {
}
