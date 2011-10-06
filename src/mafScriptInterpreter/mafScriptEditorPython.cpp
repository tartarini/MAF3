/*
 *  mafScriptEditorPython.cpp
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafScriptEditorPython.h"
#include <mafThreadSynchronizer.h>
#include "mafScriptEditorPythonModuleManager.h"
#include <iostream>

using namespace mafScriptInterpreter;

/**
 Class Name: mafScriptEditorPythonPrivate
 Pimpl Pattern.
 */
class mafScriptInterpreter::mafScriptEditorPythonPrivate {
public:
    unsigned int thread_level;

    PyThreadState* thread_state; ///< thread state for Python interpreter
};

mafScriptEditorPython::mafScriptEditorPython(QObject *parent) : mafScriptEditor(parent), m_PrivateClassPointer(new mafScriptEditorPythonPrivate) {
    m_PrivateClassPointer->thread_level = 0;
    m_PrivateClassPointer->thread_state = 0;

    //int stat;

    Py_Initialize();
    
std::string stdOutErr = "import sys\n\
class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n"; //this is python code to redirect stdouts/stderr
    
    m_PythonModule = PyImport_AddModule("__main__"); //create main module
    int res = PyRun_SimpleString(stdOutErr.c_str()); //invoke code to redirect
    
    // -- Setting up utilities
    //interpret("import sys", &stat);
    
    // -- Setting up managed modules
    mafScriptEditorPythonModuleManager::instance()->initialize(this);

    registerPrompt(&prompt);
}

mafScriptEditorPython::~mafScriptEditorPython(void) {
    Py_Finalize();

    delete m_PrivateClassPointer;

    m_PrivateClassPointer = NULL;
}

void mafScriptEditorPython::registerVariable(bool &var, QString name, QString description)  {

}

void mafScriptEditorPython::registerVariable(int &var, QString name, QString description)  {

}

void mafScriptEditorPython::registerVariable(double &var, QString name, QString description)  {

}

void mafScriptEditorPython::registerVariable(char * &var, QString name, QString description)  {

}

void mafScriptEditorPython::unregisterVariable(QString name) {

}

void mafScriptEditorPython::allowThreads(void) {
    m_PrivateClassPointer->thread_level--;

    if (m_PrivateClassPointer->thread_level == 0)
        m_PrivateClassPointer->thread_state = PyEval_SaveThread();
}

void mafScriptEditorPython::blockThreads(void) {
    if((m_PrivateClassPointer->thread_level == 0) && (m_PrivateClassPointer->thread_state))
        PyEval_RestoreThread(m_PrivateClassPointer->thread_state);

    m_PrivateClassPointer->thread_level++;
}

QString mafScriptEditorPython::interpret(const QString& command, int *stat) {
    blockThreads();
    
    PyObject *catcher = PyObject_GetAttrString(m_PythonModule,"catchOutErr"); //get our catchOutErr created above
    PyObject* empty = PyString_FromString("");
    PyObject_SetAttrString(catcher,"value", empty);
    
    QByteArray ba = command.toAscii();
    switch(PyRun_SimpleString(ba.constData())) {
        case  0:
            *stat = Status_Ok;
        break;
        case -1:
            *stat = Status_Error;
        break;
        default:
        break;
    }

    catcher = PyObject_GetAttrString(m_PythonModule,"catchOutErr"); //get our catchOutErr created above
    PyErr_Print(); //make python print any errors
    
    QString res("");

    PyObject *output = PyObject_GetAttrString(catcher,"value"); //get the stdout and stderr from our catchOutErr object

    res = PyString_AsString(output);

    mafCore::mafThreadSynchronizer::instance()->wake();

    allowThreads();

    return res;
}

QString mafScriptEditorPython::interpret(const QString& command, const QStringList& args, int *stat) {
    QString result = "";

    blockThreads();

    PyObject *modname = PyString_FromString("__main__");
    PyObject *mod = PyImport_Import(modname);
    if (mod) {
	    PyObject *mdict = PyModule_GetDict(mod);
        QByteArray ba = command.toAscii();
	    PyObject *func = PyDict_GetItemString(mdict, ba.constData());
	    if (func) {
	        if (PyCallable_Check(func)) {
		        PyObject *args = PyTuple_New(0);
		        PyObject *rslt = PyObject_CallObject(func, args);
		            if (rslt) {
		            result = QString(PyString_AsString(rslt));
		            Py_XDECREF(rslt);
		        }
		        Py_XDECREF(args);
	        }
	    }        
	    Py_XDECREF(mod);
    }
    Py_XDECREF(modname);

    allowThreads();

    return result;
}

char *mafScriptEditorPython::prompt(void) {
    QByteArray ba = QString("\033[01;35mpython\033[00m:\033[01;34m~\033[00m$ ").toAscii();
    return ba.data();
}

