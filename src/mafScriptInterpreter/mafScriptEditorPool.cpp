/*
 *  mafScriptEditorPool.cpp
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafScriptEditorPython.h>
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
    if(!m_PrivateClassPointer->interpreters.contains("console") || !m_PrivateClassPointer->interpreters.value("console"))
        m_PrivateClassPointer->interpreters.insert("console", new mafScriptEditorPython);

    return m_PrivateClassPointer->interpreters.value("console");
}

mafScriptEditor *mafScriptEditorPool::python(void)
{
    if(!m_PrivateClassPointer->interpreters.contains("python") || !m_PrivateClassPointer->interpreters.value("python")) {
	    m_PrivateClassPointer->interpreters.insert("python", new mafScriptEditorPython);
    }

    return m_PrivateClassPointer->interpreters.value("python");
}

mafScriptEditorPool::mafScriptEditorPool(void) : QObject(), m_PrivateClassPointer(new mafScriptEditorPoolPrivate)
{

}

mafScriptEditorPool::~mafScriptEditorPool(void)
{
    Q_FOREACH(mafScriptEditor *interpreter, m_PrivateClassPointer->interpreters.values())
	delete interpreter;

    delete m_PrivateClassPointer;

    m_PrivateClassPointer = NULL;
}

mafScriptEditorPool *mafScriptEditorPool::m_instance = NULL;
