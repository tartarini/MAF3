/*
 *  mafScriptEditorPool.cpp
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafScriptEditorPython.h>
#include "mafScriptEditorPool.h"

#include "mafScriptEditorPython.h"

#include <mafObjectFactory.h>

using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafScriptEditorPoolPrivate {
public:
    QMap<QString, QPointer<mafScriptEditor> > interpreters; ///< Map containing the mafScriptEditor instances asociated with the interpreter's name.
    QHash<QString, QString> m_RegisteredInterpreters; ///< Hash containing the association between the interpreter's name and interpreter's class type usefull to ask the mafObjectFactory to instantiate the interpreter editor class.
};

mafScriptEditorPool *mafScriptEditorPool::instance(void) {
    if(!m_instance) {
	    m_instance = new mafScriptEditorPool;
    }

    return m_instance;
}

void mafScriptEditorPool::shutdown() {

}

mafScriptEditor *mafScriptEditorPool::console(QString type) {
    if(!m_PrivateClassPointer->interpreters.contains(type) || !m_PrivateClassPointer->interpreters.value(type)) {
        if (m_PrivateClassPointer->m_RegisteredInterpreters.contains(type)) {
            QString ct = m_PrivateClassPointer->m_RegisteredInterpreters.value(type);
            mafCore::mafObjectBase *obj = mafNEWFromString(ct);
            mafScriptEditor *editor = qobject_cast<mafScriptEditor*>(obj);
            m_PrivateClassPointer->interpreters.insert(type, editor);
        } else {
            return NULL;
        }
    }

    return m_PrivateClassPointer->interpreters.value(type);
}

void mafScriptEditorPool::registerEditor(QString type, QString classType) {
    m_PrivateClassPointer->m_RegisteredInterpreters.insert(type, classType);
}

mafScriptEditorPool::mafScriptEditorPool(void) : QObject(), m_PrivateClassPointer(new mafScriptEditorPoolPrivate) {
    mafRegisterObject(mafScriptInterpreter::mafScriptEditorPython);
    registerEditor("python", "mafScriptInterpreter::mafScriptEditorPython");
}

mafScriptEditorPool::~mafScriptEditorPool(void) {
    mafUnregisterObject(mafScriptInterpreter::mafScriptEditorPython);

    Q_FOREACH(mafScriptEditor *interpreter, m_PrivateClassPointer->interpreters.values()) {
        mafDEL(interpreter);
    }

    delete m_PrivateClassPointer;
    m_PrivateClassPointer = NULL;
}

mafScriptEditorPool *mafScriptEditorPool::m_instance = NULL;
