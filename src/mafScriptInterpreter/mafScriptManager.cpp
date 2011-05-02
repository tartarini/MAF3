/* mafScriptManager.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Aug  4 21:03:39 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Oct 14 21:15:22 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 49
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "mafScriptManager.h"

#include <mafLog.h>

#include <mafScriptEditor.h>
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
#include <mafScriptEditorPython.h>
#endif

using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafScriptManagerPrivate
{
public:
    QString script_path;
    QString module_path;

    QHash<QString, mafScriptEditor *> loaders;
};

mafScriptManager *mafScriptManager::instance(void)
{
    static mafScriptManager instance;

    return &instance;
}

void mafScriptManager::initialize(void)
{
    this->readSettings();
}

void mafScriptManager::uninitialize(void)
{
    this->writeSettings();
}

void mafScriptManager::readSettings(void)
{
    QSettings settings;
    settings.beginGroup("scripts");
    d->script_path = settings.value("script_path", "scripts").toString();
    d->module_path = settings.value("module_path", "modules").toString();
    settings.endGroup();

    if(d->module_path.isEmpty() && d->script_path.isEmpty()) {
        mafLog::warning() << "Your maf config does not seem to be set correctly.";
        mafLog::warning() << "Please set scripts.script_path.";
        mafLog::warning() << "Please set scripts.module_path.";
    }
}

void mafScriptManager::writeSettings(void)
{
    QSettings settings;
    settings.beginGroup("scripts");
    settings.setValue("script_path", d->script_path);
    settings.setValue("module_path", d->module_path);
    settings.endGroup();
}

QString mafScriptManager::scriptPath(void) const
{
    return d->script_path;
}

QString mafScriptManager::modulePath(void) const
{
    return d->module_path;
}

mafScriptManager::mafScriptManager(void) : d(new mafScriptManagerPrivate)
{

}

mafScriptManager::~mafScriptManager(void)
{
    delete d;

    d = NULL;
}

mafScriptEditor *mafScriptManager::loadScript(const QString& path)
{
#if defined(HAVE_SWIG) && defined(HAVE_TCL) && defined(HAVE_PYTHON)
    mafScriptEditor *interpreter;

    if(path.endsWith("py")) 
        interpreter = new mafScriptEditorPython;

    interpreter->load(path);

    d->loaders.insert(path, interpreter);

    return interpreter;
#else
    return NULL;
#endif
}

mafScriptEditor *mafScriptManager::unloadScript(const QString& path)
{
#if defined(HAVE_SWIG) && defined(HAVE_TCL) && defined(HAVE_PYTHON)
    mafScriptEditor *interpreter = d->loaders.value(path);

    d->loaders.remove(path);

    return interpreter;
#else
    return NULL;
#endif
}

mafScriptEditor *mafScriptManager::reloadScript(const QString& path)
{
#if defined(HAVE_SWIG) && defined(HAVE_TCL) && defined(HAVE_PYTHON)
    this->unloadScript(path);

    return this->loadScript(path);
#else
    return NULL;
#endif
}