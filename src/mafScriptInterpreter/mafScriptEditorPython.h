/* mafScriptEditorPython.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 26 16:28:12 2008 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Feb 11 14:09:07 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 24
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFSCRIPTINTERPRETERPYTHON_H
#define MAFSCRIPTINTERPRETERPYTHON_H

#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

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

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPython : public mafScriptEditor
{
    Q_OBJECT

public:
     mafScriptEditorPython(QObject *parent = 0);
    ~mafScriptEditorPython(void);

    virtual void registerVariable(bool   &var, QString name, QString description = "");
    virtual void registerVariable(int    &var, QString name, QString description = "");
    virtual void registerVariable(double &var, QString name, QString description = "");
    virtual void registerVariable(char * &var, QString name, QString description = "");

    virtual void unregisterVariable(QString name);

    void allowThreads(void);
    void blockThreads(void);

public slots:
    virtual QString interpret(const QString& command, int *stat);
    virtual QString interpret(const QString& command, const QStringList& args, int *stat);

signals:
    void interpreted(const QString& result, int *stat);
    
protected:
    static char *prompt(void);

private:
    mafScriptEditorPythonPrivate *d;
    PyObject *pModule;
};

// /////////////////////////////////////////////////////////////////
// mafScriptEditorPythonModuleManager
// /////////////////////////////////////////////////////////////////

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPythonModuleManager : public QObject
{
    typedef int (*mafScriptEditorModuleInitializer)(void);

public:
    static mafScriptEditorPythonModuleManager* instance(void) {
	if(!m_instance)
	    m_instance = new mafScriptEditorPythonModuleManager;

	return m_instance;
    }

    void initialize(mafScriptEditorPython *interpreter) {
	int stat;

        interpreter->blockThreads();

	foreach(mafScriptEditorModuleInitializer initializer, initializers)
	    initializer();

        interpreter->allowThreads();

	foreach(QString command, commands) 
	    interpreter->interpret(command, &stat);
    }

    void registerInitializer(mafScriptEditorModuleInitializer initializer) {
	initializers << initializer;
    }

    void registerCommand(QString command) {
	commands << command;
    }

protected:
     mafScriptEditorPythonModuleManager(void) {}
    ~mafScriptEditorPythonModuleManager(void) {}

private:
    QList<mafScriptEditorModuleInitializer> initializers;
    QList<QString> commands;

    static mafScriptEditorPythonModuleManager *m_instance;
};

} // end namespace
    
#endif
