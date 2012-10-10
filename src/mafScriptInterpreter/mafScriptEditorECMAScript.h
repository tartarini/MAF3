/*
 *  mafScriptEditorECMAScript.h
 *  mafScriptEditor
 *
 *  Created by Paolo Quadrani on 21/03/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTEDITORECMASCRIPT_H
#define MAFSCRIPTEDITORECMESCRIPT_H

#include "mafScriptInterpreter_global.h"

#include <mafScriptEditor.h>
#include <QScriptEngine>

namespace mafScriptInterpreter {

/**
 Class name: mafScriptEditorECMAScript
 This class represents the bridge class to ECMAScript-262 editor.
 */
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorECMAScript : public mafScriptEditor
{
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafScriptInterpreter::mafScriptEditor);

public:
    /// Object Constructor.
     mafScriptEditorECMAScript(const QString code_location = "");

    /// Register a boolean type variable with value, name and a description.
    /*virtual*/ void registerVariable(bool   &var, QString name, QString description = "");
    /// Register a integer type variable with value, name and a description.
    /*virtual*/ void registerVariable(int    &var, QString name, QString description = "");
    /// Register a double type variable with value, name and a description.
    /*virtual*/ void registerVariable(double &var, QString name, QString description = "");
    /// Register a char type variable with value, name and a description.
    /*virtual*/ void registerVariable(char * &var, QString name, QString description = "");
    /// Register a QObject type variable with value, name and a description.
    /*virtual*/ void registerVariable(QObject &var, QString name, QString description = "");
    /// Return the value of a registered variable.
    QScriptValue variable(QString name);

    /// Unregister a variable.
    /*virtual*/ void unregisterVariable(QString name);

public Q_SLOTS:
    /// Setup the text highlighter.
    /*virtual*/ void setupHighliter(QTextDocument *doc);

    /// Interpret command string.
    /*virtual*/ QString interpret(const QString& command, int *stat);

    /// Interpret command string with parameters.
    /*virtual*/ QString interpret(const QString& command, const QStringList& args, int *stat);
    
protected:
    /// Object Destructor.
    ~mafScriptEditorECMAScript(void);

    /// Return prompt string.
    static char *prompt(void);

private:
    QScriptEngine m_Engine; /// ECMAScript engine.
};

} // end namespace
    
#endif // MAFSCRIPTEDITORPYTHONECMASCRIPT_H
