/*
 *  mafScriptEditorECMAScript.cpp
 *  mafScriptEditor
 *
 *  Created by Paolo Quadrani on 21/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafScriptEditorECMAScript.h"
#include <mafThreadSynchronizer.h>
//#include "mafScriptEditorPythonModuleManager.h"
#include <iostream>

using namespace mafScriptInterpreter;

mafScriptEditorECMAScript::mafScriptEditorECMAScript(const QString code_location) : mafScriptEditor(code_location) {
    registerPrompt(&prompt);
}

mafScriptEditorECMAScript::~mafScriptEditorECMAScript(void) {
}

void mafScriptEditorECMAScript::registerVariable(bool &var, QString name, QString description)  {
    m_Engine.globalObject().setProperty(name, var);
}

void mafScriptEditorECMAScript::registerVariable(int &var, QString name, QString description)  {
    m_Engine.globalObject().setProperty(name, var);
}

void mafScriptEditorECMAScript::registerVariable(double &var, QString name, QString description)  {
    m_Engine.globalObject().setProperty(name, var);
}

void mafScriptEditorECMAScript::registerVariable(char * &var, QString name, QString description)  {
    m_Engine.globalObject().setProperty(name, var);
}

void mafScriptEditorECMAScript::registerVariable(QObject &var, QString name, QString description) {
    QScriptValue scriptObject = m_Engine.newQObject(&var);
    m_Engine.globalObject().setProperty(name, scriptObject);
}

QScriptValue mafScriptEditorECMAScript::variable(QString name) {
    return m_Engine.globalObject().property(name);
}

void mafScriptEditorECMAScript::unregisterVariable(QString name) {
}

QString mafScriptEditorECMAScript::interpret(const QString& command, int *stat) {
    QScriptSyntaxCheckResult chkRes = m_Engine.checkSyntax(command);
    int s = chkRes.state();
    *stat = Status_Error;
    if (s != QScriptSyntaxCheckResult::Valid) {
        if (s == QScriptSyntaxCheckResult::Error) {
            qWarning() << mafTr("Syntax errors at line: ") << chkRes.errorLineNumber() << " with message:" << chkRes.errorMessage();
        }
        if (s == QScriptSyntaxCheckResult::Intermediate) {
            qWarning() << mafTr("Incomplete command");
        }
        return "";
    }
    QScriptValue result = m_Engine.evaluate(command);
    if (m_Engine.hasUncaughtException()) {
        int line = m_Engine.uncaughtExceptionLineNumber();
        qWarning() << mafTr("uncaught exception at line") << line << ":" << result.toString();
        return "";
    }
    *stat = Status_Ok;

    return result.toString();
}

QString mafScriptEditorECMAScript::interpret(const QString& command, const QStringList& args, int *stat) {
    QScriptValueList argsScript;
    Q_FOREACH(QString arg, args) {
        argsScript << arg;
    }
    *stat = Status_Error;
    QScriptValue scriptToExecute = variable(command);
    *stat = scriptToExecute.isValid() ? Status_Ok : Status_Error;
    QScriptValue threeAgain = scriptToExecute.call(QScriptValue(), argsScript);
    *stat = threeAgain.isValid() ? Status_Ok : Status_Error;
    return threeAgain.toString();
}

char *mafScriptEditorECMAScript::prompt(void) {
    QByteArray ba = QString("\033[01;35mECMAScript\033[00m:\033[01;34m~\033[00m$ ").toAscii();
    return ba.data();
}
