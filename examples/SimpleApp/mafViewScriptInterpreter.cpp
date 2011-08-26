#include "mafViewScriptInterpreter.h"
#include <mafInterpreterConsole.h>

using namespace mafResources;
using namespace mafScriptInterpreter;

mafViewScriptInterpreter::mafViewScriptInterpreter(const QString code_location) : mafView(code_location) {
    m_RenderWidget = new mafInterpreterConsole();
    m_RenderWidget->setObjectName("ScriptInterpreter");
    ((mafInterpreterConsole*)m_RenderWidget)->readSettings();
}

mafViewScriptInterpreter::~mafViewScriptInterpreter() {
}
