/* mafScriptEditorPool.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Jan 26 09:48:03 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Thu Oct 14 21:13:17 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 50
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <mafScriptEditor.h>
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
#include <mafScriptEditorPython.h>
#endif

#include "mafScriptEditorPool.h"

using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafScriptEditorPoolPrivate
{
public:
    QMap<QString, QPointer<mafScriptEditor> > interpreters;
};

mafScriptEditorPool *mafScriptEditorPool::instance(void)
{
    if(!m_instance)
	m_instance = new mafScriptEditorPool;

    return m_instance;
}

mafScriptEditor *mafScriptEditorPool::console(QString type)
{
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON) && defined(HAVE_TCL)
    if(!d->interpreters.contains("console") || !d->interpreters.value("console"))
        d->interpreters.insert("console", new mafScriptEditorPython);

    return d->interpreters.value("console");
#else
    return NULL;
#endif
}

mafScriptEditor *mafScriptEditorPool::python(void)
{
#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
    if(!d->interpreters.contains("python") || !d->interpreters.value("python"))
	d->interpreters.insert("python", new mafScriptEditorPython);

    return d->interpreters.value("python");
#else
    return NULL;
#endif
}

mafScriptEditorPool::mafScriptEditorPool(void) : QObject(), d(new mafScriptEditorPoolPrivate)
{

}

mafScriptEditorPool::~mafScriptEditorPool(void)
{
    foreach(mafScriptEditor *interpreter, d->interpreters.values())
	delete interpreter;

    delete d;

    d= NULL;
}

mafScriptEditorPool *mafScriptEditorPool::m_instance = NULL;
