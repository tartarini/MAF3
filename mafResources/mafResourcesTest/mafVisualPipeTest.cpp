/*
 *  mafVisualPipeTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafVisualPipe.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: mafVisualPipeCustom
 This class implements the data pipe to be tested.
 */
class  testVisualPipeCustomForVisualPipe : public  mafVisualPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafVisualPipe);

public:
    /// Object constructor.
    testVisualPipeCustomForVisualPipe(const mafString code_location = "");

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

    /// Return the string variable initializated and updated from the data pipe.
    mafString pipeline() {return m_PipeLine;}

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    mafString m_PipeLine; ///< Test Var.
};

testVisualPipeCustomForVisualPipe::testVisualPipeCustomForVisualPipe(const mafString code_location) : mafVisualPipe(code_location), m_PipeLine("") {
}

void testVisualPipeCustomForVisualPipe::createPipe() {
    m_PipeLine = "Created";
}

void testVisualPipeCustomForVisualPipe::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(mafString::number(t));
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafVisualPipeTest
 This class implements the test suite for mafVisualPipe.
 */
class mafVisualPipeTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_VisualPipe = mafNEW(testVisualPipeCustomForVisualPipe);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VisualPipe);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafVisualPipe allocation test case.
    void mafVisualPipeAllocationTest();
    /// Test the creation and update methods..
    void mafVisualPipeCreationAndUpdateTest();

private:
    testVisualPipeCustomForVisualPipe *m_VisualPipe; ///< Test var.
};

void mafVisualPipeTest::mafVisualPipeAllocationTest() {
    QVERIFY(m_VisualPipe != NULL);
}

void mafVisualPipeTest::mafVisualPipeCreationAndUpdateTest() {
    mafString res("Created");
    m_VisualPipe->createPipe();

    QCOMPARE(m_VisualPipe->pipeline(), res);

    res = "Updated1";
    m_VisualPipe->updatePipe(1);
    QCOMPARE(m_VisualPipe->pipeline(), res);
}

MAF_REGISTER_TEST(mafVisualPipeTest);
#include "mafVisualPipeTest.moc"
