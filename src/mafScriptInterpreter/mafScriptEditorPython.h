/*
 *  mafScriptEditorPython.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTEDITORPYTHON_H
#define MAFSCRIPTEDITORPYTHON_H

#include "mafScriptInterpreter_global.h"

#include <mafScriptEditor.h>

#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE

#ifdef _DEBUG
#undef _DEBUG
#define READD_DEBUG
#endif // _DEBUG
#include <Python.h>
#ifdef READD_DEBUG
#define _DEBUG
#undef READD_DEBUG
#endif // READD_DEBUG


namespace mafScriptInterpreter {

class mafScriptEditorPythonPrivate;

/**
 Class name: mafScriptEditorPython
 This class represents the bridge class to python editor.
 */
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPython : public mafScriptEditor
{
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafScriptInterpreter::mafScriptEditor);

public:
    /// Object Constructor.
     mafScriptEditorPython(const QString code_location = "");

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

    /// Unregister a variable.
    /*virtual*/ void unregisterVariable(QString name);

    /// Permits all threads to run.
    void allowThreads(void);
    /// Execute threads in blocking mode.
    void blockThreads(void);

public Q_SLOTS:
    /// Setup the text highlighter.
    /*virtual*/ void setupHighliter(QTextDocument *doc);

    /// Interpret command string.
    /*virtual*/ QString interpret(const QString& command, int *stat);
    
    /// Interpret command string with parameters.
    /*virtual*/ QString interpret(const QString& command, const QStringList& args, int *stat);
    
protected:
    /// Object Destructor.
    ~mafScriptEditorPython(void);

    /// Return prompt string.
    static char *prompt(void);

private:
    mafScriptEditorPythonPrivate *m_PrivateClassPointer; /// pimpl pattern.
    PyObject *m_PythonModule; /// temporary python module.
};

} // end namespace
    
#endif // MAFSCRIPTEDITORPYTHON_H
