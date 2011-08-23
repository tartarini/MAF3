#include "mafViewScriptInterpreter.h"
#include <mafInterpreterEditor.h>

using namespace mafResources;
using namespace mafScriptInterpreter;

mafViewScriptInterpreter::mafViewScriptInterpreter(const QString code_location) : mafView(code_location) {
    m_RenderWidget = new mafInterpreterEditor();
    m_RenderWidget->setObjectName("ScriptInterpreter");
    ((mafInterpreterEditor*)m_RenderWidget)->readSettings();
}

mafViewScriptInterpreter::~mafViewScriptInterpreter() {
}
