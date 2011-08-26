#include "mafViewScriptInterpreterPython.h"
#include <mafInterpreterConsole.h>
#include <mafScriptEditorPython.h>
#include <mafScriptEditorPool.h>

using namespace mafScriptInterpreter;

mafViewScriptInterpreterPython::mafViewScriptInterpreterPython(const QString code_location) : mafViewScriptInterpreter(code_location) {
}

mafViewScriptInterpreterPython::~mafViewScriptInterpreterPython() {
}

void mafViewScriptInterpreterPython::create() {
    mafScriptEditor *interpreter = mafScriptEditorPool::instance()->python();

    ((mafInterpreterConsole*)m_RenderWidget)->registerInterpreter(interpreter);
}