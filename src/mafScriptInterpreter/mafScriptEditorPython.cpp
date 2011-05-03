/* mafScriptEditorPython.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 26 16:29:02 2008 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Feb 11 14:11:07 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 261
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <mafScriptEditorPython.h>

#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE

#include <Python.h>

using namespace mafScriptInterpreter;

#include <iostream>

static bool redirection_occured;

static PyObject* redirector_init(PyObject *, PyObject *)
{
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* redirector_write(PyObject *, PyObject *args)
{
    char* output;
    PyObject *selfi;

    if (!PyArg_ParseTuple(args, "Os", &selfi, &output))
 	return NULL;

    QString text(output);

    if(!text.simplified().isEmpty())
        std::cout << text.toAscii().constData() << std::flush << std::endl;

    redirection_occured = true;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef redirectorMethods[] =
{
    {"__init__", redirector_init, METH_VARARGS,
     "initialize the stdout/err redirector"},
    {"write", redirector_write, METH_VARARGS,
     "implement the write method to redirect stdout/err"},
    {NULL,NULL,0,NULL}
};

static PyMethodDef ModuleMethods[] = { {NULL,NULL,0,NULL} };

void init_redirector(void)
{
    PyMethodDef *def;

    PyObject *module = Py_InitModule("redirector", ModuleMethods);
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("redirector");
    PyObject *fooClass = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "redirector", fooClass);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(fooClass);

    for (def = redirectorMethods; def->ml_name != NULL; def++) {
        PyObject *func = PyCFunction_New(def, NULL);
        PyObject *method = PyMethod_New(func, NULL, fooClass);
        PyDict_SetItemString(classDict, def->ml_name, method);
        Py_DECREF(func);
        Py_DECREF(method);
    }
}

// /////////////////////////////////////////////////////////////////
// mafScriptEditorPythonPrivate
// /////////////////////////////////////////////////////////////////

class mafScriptInterpreter::mafScriptEditorPythonPrivate
{
public:
    unsigned int thread_level;

    PyThreadState* thread_state;
};

// /////////////////////////////////////////////////////////////////
// mafScriptEditorPython
// /////////////////////////////////////////////////////////////////

mafScriptEditorPython::mafScriptEditorPython(QObject *parent) : mafScriptEditor(parent), d(new mafScriptEditorPythonPrivate)
{
    d->thread_level = 0;
    d->thread_state = 0;

    int stat;

    Py_Initialize();
    int argc = 0;

    // -- Redirection
    init_redirector();
    interpret("import sys",                           &stat);
    interpret("import redirector",                    &stat);
    interpret("sys.stdout = redirector.redirector()", &stat);
    interpret("sys.stderr = sys.stdout",              &stat);

    // -- Setting up utilities
    interpret("import sys", &stat);
    interpret("sys.path.append(\"" + mafScriptManager::instance()->modulePath() + "\")", &stat);

    // -- Setting up managed modules
    mafScriptEditorPythonModuleManager::instance()->initialize(this);

    registerPrompt(&prompt);
}

mafScriptEditorPython::~mafScriptEditorPython(void)
{
    Py_Finalize();

    delete d;

    d = NULL;
}

void mafScriptEditorPython::registerVariable(bool &var, QString name, QString description) 
{

}

void mafScriptEditorPython::registerVariable(int &var, QString name, QString description) 
{

}

void mafScriptEditorPython::registerVariable(double &var, QString name, QString description) 
{

}

void mafScriptEditorPython::registerVariable(char * &var, QString name, QString description) 
{

}

void mafScriptEditorPython::unregisterVariable(QString name)
{

}

void mafScriptEditorPython::allowThreads(void)
{
    d->thread_level--;

    if (d->thread_level == 0)
        d->thread_state = PyEval_SaveThread();
}

void mafScriptEditorPython::blockThreads(void)
{    
    if((d->thread_level == 0) && (d->thread_state))
        PyEval_RestoreThread(d->thread_state);

    d->thread_level++;
}

QString mafScriptEditorPython::interpret(const QString& command, int *stat)
{
    redirection_occured = false;
    blockThreads();

    switch(PyRun_SimpleString(command.toAscii().constData())) {
    case  0: *stat = Status_Ok;    break;
    case -1: *stat = Status_Error; break;
    default: break;
    }

    if(!redirection_occured)
        emit interpreted("", stat);

    mafScriptEditorSynchronizer::instance()->wake();

    allowThreads();

    return "";
}

QString mafScriptEditorPython::interpret(const QString& command, const QStringList& args, int *stat)
{
    QString result = "";

    blockThreads();

    PyObject *modname = PyString_FromString("__main__");
    PyObject *mod = PyImport_Import(modname);
    if (mod) {
	PyObject *mdict = PyModule_GetDict(mod);
	PyObject *func = PyDict_GetItemString(mdict, command.toAscii().constData());
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

char *mafScriptEditorPython::prompt(void)
{
    return QString("\033[01;35mpython\033[00m:\033[01;34m~\033[00m$ ").toAscii().data();
}

// /////////////////////////////////////////////////////////////////
// mafScriptEditorPythonModuleManager
// /////////////////////////////////////////////////////////////////

MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPythonModuleManager *mafScriptEditorPythonModuleManager::m_instance = NULL;
