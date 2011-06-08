/*
 *  mafPipeVisualTest.cpp
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
#include <mafPipeVisual.h>
#include <mafVME.h>
#include <mafProxy.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: mafPipeVisualCustom
 This class implements the data pipe to be tested.
 */
class  testVisualPipeCustomForVisualPipe : public  mafPipeVisual {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
    /// Object constructor.
    testVisualPipeCustomForVisualPipe(const QString code_location = "");

    /// Return the string variable initialized and updated from the data pipe.
    QString pipeline() {return m_PipeLine;}
    
    /// set the selection pipe for viualization.
    virtual void setPipeVisualSelection(mafPipeVisual *pipeVisualSelection);

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    QString m_PipeLine; ///< Test var.
    mafCore::mafProxy<QString> m_Container; ///< Test var.

};

testVisualPipeCustomForVisualPipe::testVisualPipeCustomForVisualPipe(const QString code_location) : mafPipeVisual(code_location), m_PipeLine("Created") {
    m_Output = &m_Container;
}

void testVisualPipeCustomForVisualPipe::setPipeVisualSelection(mafPipeVisual *pipeVisualSelection) {
    Q_UNUSED(pipeVisualSelection);
}

void testVisualPipeCustomForVisualPipe::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(QString::number(t));

    m_Container = new QString;
    m_Container.externalData()->append(m_PipeLine);
}


//------------------------------------------------------------------------------------------

/**
 Class name: mafPipeVisualTest
 This class implements the test suite for mafPipeVisual.
 */
class mafPipeVisualTest : public QObject {
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
        mafEventBus::mafEventBusManager::instance()->shutdown();
    }

    /// mafPipeVisual allocation test case.
    void mafPipeVisualAllocationTest();
    /// Test the creation and update methods..
    void mafPipeVisualCreationAndUpdateTest();
private:
    testVisualPipeCustomForVisualPipe *m_VisualPipe; ///< Test var.
};

void mafPipeVisualTest::mafPipeVisualAllocationTest() {
    QVERIFY(m_VisualPipe != NULL);
}

void mafPipeVisualTest::mafPipeVisualCreationAndUpdateTest() {
    QString res("Created");
    QCOMPARE(m_VisualPipe->pipeline(), res);

    res = "Updated1";
    m_VisualPipe->updatePipe(1);
    QCOMPARE(m_VisualPipe->pipeline(), res);
}

MAF_REGISTER_TEST(mafPipeVisualTest);
#include "mafPipeVisualTest.moc"
