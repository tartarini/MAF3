/*
 *  mafScriptEngine.cpp
 *  mafScriptInterpreter
 *
 *  Created by Paolo Quadrani on 18/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafScriptEngine.h"
#include "mafScriptEditorPython.h"
#include "mafScriptEditorPool.h"

#include <mafIdProvider.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafScriptInterpreter;

mafScriptEngine* mafScriptEngine::instance() {
    // Create the instance of the script engine.
    static mafScriptEngine instanceScriptEngine;
    return &instanceScriptEngine;
}

void mafScriptEngine::shutdown() {
    Q_FOREACH(mafObject *obj, m_ObjectsWithScript) {
        obj->release();
    }
    m_ObjectsWithScript.clear();
}

mafScriptEngine::mafScriptEngine(const QString code_location) : mafObjectBase(code_location) {
    // Create the Topic needed to register the object containing the script.
    mafId register_object_id = mafIdProvider::instance()->createNewId("maf.local.script.register");
    if(register_object_id != -1) {
        mafRegisterLocalSignal("maf.local.script.register", this, "registerObjectToManagerSignal(mafCore::mafObject *)");
        mafRegisterLocalCallback("maf.local.script.register", this, "registerObjectToManager(mafCore::mafObject *)");
    }

    // Create the Topic needed to un-register the object containing the script.
    mafId unregister_object_id = mafIdProvider::instance()->createNewId("maf.local.script.unregister");
    if(unregister_object_id != -1) {
        mafRegisterLocalSignal("maf.local.script.unregister", this, "unregisterObjectFromManagerSignal(mafCore::mafObject *)");
        mafRegisterLocalCallback("maf.local.script.unregister", this, "unregisterObjectFromManager(mafCore::mafObject *)");
    }

    // Create the Topic needed to execute the script associated with the object.
    mafId execute_script_id = mafIdProvider::instance()->createNewId("maf.local.script.execute");
    if(execute_script_id != -1) {
        mafRegisterLocalSignal("maf.local.script.execute", this, "executeScriptSignal(mafCore::mafObject *)");
        mafRegisterLocalCallback("maf.local.script.execute", this, "executeScript(mafCore::mafObject *)");
    }

    m_Interpreter = mafScriptEditorPool::instance()->python();
}

mafScriptEngine::~mafScriptEngine() {
    // Unregister callbacks...
    mafUnregisterLocalCallback("maf.local.script.register", this, "registerObjectToManager(mafCore::mafObject *)");
    mafUnregisterLocalCallback("maf.local.script.unregister", this, "unregisterObjectFromManager(mafCore::mafObject *)");
    mafUnregisterLocalCallback("maf.local.script.execute", this, "executeScript(mafCore::mafObject *)");

    // Unregister signals...
    mafUnregisterLocalSignal("maf.local.script.register", this, "registerObjectToManagerSignal(mafCore::mafObject *)");
    mafUnregisterLocalSignal("maf.local.script.unregister", this, "unregisterObjectFromManagerSignal(mafCore::mafObject *)");
    mafUnregisterLocalSignal("maf.local.script.execute", this, "executeScriptSignal(mafCore::mafObject *)");

    // Removed IDs...
    mafIdProvider *provider = mafIdProvider::instance();
    provider->removeId("maf.local.script.register");
    provider->removeId("maf.local.script.unregister");
    provider->removeId("maf.local.script.execute");
}

void mafScriptEngine::registerObjectToManager(mafCore::mafObject *objectWithScript) {
    if (objectWithScript) {
        objectWithScript->retain();
        m_ObjectsWithScript.append(objectWithScript);
    }
}
	
void mafScriptEngine::unregisterObjectFromManager(mafCore::mafObject *objectWithScript) {
    if (objectWithScript && m_ObjectsWithScript.contains(objectWithScript)) {
        m_ObjectsWithScript.removeOne(objectWithScript);
        objectWithScript->release();
    }
}

bool mafScriptEngine::executeScriptOnObject(mafCore::mafObject *objectWithScript) {
    int stat = 0;
    const QVariantList *scriptsToExecute;
    // Execute the particular script associated with the given object.
    scriptsToExecute = objectWithScript->scriptList();
    Q_FOREACH(QVariant script, *scriptsToExecute) {
        // m_Interpreter should be a local variable and ask the interpreterPool the
        // type of interpreter to use according to the interpreter type written inside 
        // the dictionary
        const QVariantHash dic = script.toHash();
        int scriptType = dic.value(mafScriptTypeKey).toInt();
        if (scriptType == mafScriptTypeStringScript) {
            m_Interpreter->interpret(dic.value(mafScriptKey).toString(), &stat);
        } else {
            stat = m_Interpreter->load(dic.value(mafScriptKey).toString()) ? 0 : 1;
        }
        if (stat) {
            break;
        }
    }
    return stat == 0;
}

bool mafScriptEngine::executeScript(mafCore::mafObject *objectWithScript) {
    bool resultOk(false);
    
    if (objectWithScript) {
        resultOk = executeScriptOnObject(objectWithScript);
    } else {
        // execute the script associated to all the registered objects.
        Q_FOREACH(mafObject *obj, m_ObjectsWithScript) {
            resultOk = executeScriptOnObject(obj);
            if (!resultOk) {
                break;
            }
        }
    }
    return resultOk;
}
