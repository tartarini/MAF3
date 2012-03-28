/*
 *  mafScriptEngineECMAScriptTest.cpp
 *  mafScriptInterpreterTest
 *
 *  Created by Paolo Quadrani on 22/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafScriptEditorECMAScript.h>

#include <mafMessageHandler.h>
#include <mafObjectFactory.h>

using namespace mafCore;
using namespace mafScriptInterpreter;

/**
 Class name: mafScriptEngineECMAScriptTest
 This class implements the test suite for mafScriptEditorECMAScript.
 */

//! <title>
//mafScriptEditorECMAScript
//! </title>
//! <description>
//mafScriptEditorECMAScript defines the script engine to evaluate the ECMA script.
//! </description>

class mafScriptEditorECMAScriptTest : public QObject {
    Q_OBJECT 

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();

        //! <snippet>
        m_ScriptEngine = mafNEW(mafScriptEditorECMAScript);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_ScriptEngine);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafScriptEditorECMAScript allocation test case.
    void mafScriptEngineAllocationTest();

    /// mafScriptEditorECMAScript variable registration test case.
    void mafScriptEngineRegistrationTest();

    /// mafScriptEditorECMAScript interpret script test case.
    void mafScriptEngineInterpretTest();

private:
    mafScriptEditorECMAScript *m_ScriptEngine; ///< Test var.
};

void mafScriptEditorECMAScriptTest::mafScriptEngineAllocationTest() {
    QVERIFY(m_ScriptEngine != NULL);
}

void mafScriptEditorECMAScriptTest::mafScriptEngineRegistrationTest() {
    QObject *obj = new QObject;
    obj->setObjectName("test Object");
    m_ScriptEngine->registerVariable(*obj, "myObject", "Object used to test variable registration");
    QObject *res = m_ScriptEngine->variable("myObject").toQObject();
    QVERIFY(res->objectName() == obj->objectName());

    bool varBool(true);
    m_ScriptEngine->registerVariable(varBool, "varBool", "Boolean variable");
    QVERIFY(m_ScriptEngine->variable("varBool").toBool());

    int varInt = 5;
    m_ScriptEngine->registerVariable(varInt, "varInt", "Integer variable");
    int resInt = m_ScriptEngine->variable("varInt").toInt32();
    QVERIFY(resInt == varInt);

    double varDouble = 3.5;
    m_ScriptEngine->registerVariable(varDouble, "varDouble", "Double variable");
    double resDouble = m_ScriptEngine->variable("varDouble").toVariant().toDouble();
    QVERIFY(mafEquals(resDouble, varDouble));

    QByteArray ba = obj->objectName().toAscii();
    char *varChar = ba.data();
    m_ScriptEngine->registerVariable(varChar, "varChar", "char variable");
    QString resChar = m_ScriptEngine->variable("varChar").toString();
    QVERIFY(resChar == obj->objectName());

    delete obj;
}

void mafScriptEditorECMAScriptTest::mafScriptEngineInterpretTest() {
    QString script("var myVar = 5;");
    int status = mafScriptEditor::Status_Error;
    m_ScriptEngine->interpret(script, &status);
    QVERIFY(status == mafScriptEditor::Status_Ok);

    // Define a function with arguments.
    QString scriptCube("function cube(x) { return x * x * x; }");
    // Create the argument list.
    QStringList args;
    args << "3";
    // Let the engine interpret the function.
    m_ScriptEngine->interpret(scriptCube, &status);
    // Then call the function by passing the necessary parameter list.
    QString resultScript = m_ScriptEngine->interpret("cube", args, &status);
    int cube = resultScript.toInt();
    QVERIFY(cube == 27);
}

MAF_REGISTER_TEST(mafScriptEditorECMAScriptTest);
#include "mafScriptEditorECMAScriptTest.moc"
