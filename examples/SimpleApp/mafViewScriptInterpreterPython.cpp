#include "mafViewScriptInterpreterPython.h"
#include <mafInterpreterEditor.h>
#include <mafScriptEditorPython.h>

#include <mafLog.h>

QWidget *log_output;

void tstRedirectLogHandler(mafGUI::mafLog::LogType logType, const QString& msg)
{
    QCoreApplication::postEvent(log_output, new mafGUI::mafLogEvent(logType, msg));
}

using namespace mafScriptInterpreter;

mafViewScriptInterpreterPython::mafViewScriptInterpreterPython(const QString code_location) : mafViewScriptInterpreter(code_location) {
}

mafViewScriptInterpreterPython::~mafViewScriptInterpreterPython() {
}

void mafViewScriptInterpreterPython::create() {
    mafScriptEditor *interpreter = new mafScriptEditorPython;

    log_output = ((mafInterpreterEditor*)m_RenderWidget);
    ((mafInterpreterEditor*)m_RenderWidget)->registerInterpreter(interpreter);
    ((mafInterpreterEditor*)m_RenderWidget)->registerAsHandler(tstRedirectLogHandler);
}