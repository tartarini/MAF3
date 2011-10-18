/*
 *  mafScriptEngineTest.cpp
 *  mafScriptInterpreterTest
 *
 *  Created by Paolo Quadrani on 18/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafScriptEngine.h>

#include <mafMessageHandler.h>
#include <mafObjectFactory.h>

using namespace mafCore;
using namespace mafScriptInterpreter;

/**
 Class name: mafScriptEngineTest
 This class implements the test suite for mafScriptEngine.
 */

 //! <title>
//mafScriptEngine
//! </title>
//! <description>
//mafScriptEngine defines the singleton for script execution management.
//! </description>

class mafScriptEngineTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();

        objWithScript = mafNEW(mafCore::mafObject);
        objWithoutScript = mafNEW(mafCore::mafObject);
        objNotRegistered = mafNEW(mafCore::mafObject);

        QString script("print \"Test python script\"");
        objWithScript->addScript(script);
        QString script2("print \"Test script for not registered object\"");
        objNotRegistered->addScript(script2);
        
        //! <snippet>
        m_ScriptEngine = mafScriptEngine::instance();
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(objWithScript);
        mafDEL(objWithoutScript);
        mafDEL(objNotRegistered);

        m_ScriptEngine->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafScriptEngine allocation test case.
    void mafScriptEngineAllocationTest();

    /// mafScriptEngine object registration test case.
    void mafScriptEngineRegisterTest();

    /// mafScriptEngine object execution test case.
    void mafScriptEngineExecutionTest();

    /// mafScriptEngine object un-registration test case.
    void mafScriptEngineUnregisterTest();

private:
    mafScriptEngine *m_ScriptEngine; ///< Test var.
    mafObject *objWithScript;
    mafObject *objWithoutScript;
    mafObject *objNotRegistered;
};

void mafScriptEngineTest::mafScriptEngineAllocationTest() {
    QVERIFY(m_ScriptEngine != NULL);
}

void mafScriptEngineTest::mafScriptEngineRegisterTest() {
    m_ScriptEngine->registerObjectToManager(objWithScript);
    m_ScriptEngine->registerObjectToManager(objWithoutScript);

    bool result = m_ScriptEngine->isObjectRegistered(objWithScript);
    QVERIFY(result);

    result = m_ScriptEngine->isObjectRegistered(objNotRegistered);
    QVERIFY(!result);
}

void mafScriptEngineTest::mafScriptEngineExecutionTest() {
    bool result(false);
    result = m_ScriptEngine->executeScript(objWithScript);
    QVERIFY(result);

    // Execute the script associated to the not registered object.
    result = m_ScriptEngine->executeScript(objNotRegistered);
    QVERIFY(result);

    // An object without a script attached should not create problem.
    result = m_ScriptEngine->executeScript(objWithoutScript);
    QVERIFY(result);

    // execute again all the scripts associated to the registered objects.
    result = m_ScriptEngine->executeScript();
}

void mafScriptEngineTest::mafScriptEngineUnregisterTest() {
    m_ScriptEngine->unregisterObjectFromManager(objWithScript);
    bool result(true);

    result = m_ScriptEngine->isObjectRegistered(objWithScript);
    QVERIFY(!result);

    // Object not register should not create problems. Simply will not be unregistered.
    m_ScriptEngine->unregisterObjectFromManager(objNotRegistered);

    // try to unregister NULL object.
    m_ScriptEngine->unregisterObjectFromManager(NULL);
}

MAF_REGISTER_TEST(mafScriptEngineTest);
#include "mafScriptEngineTest.moc"
