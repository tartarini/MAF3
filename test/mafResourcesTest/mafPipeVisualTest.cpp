/*
 *  mafPipeVisualTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

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

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Allows to ask the rendering engine to render the graphic scene.
    /*virtual*/ void render();

private:
    QString m_PipeLine; ///< Test var.
    mafCore::mafProxy<QString> m_Container; ///< Test var.

};

testVisualPipeCustomForVisualPipe::testVisualPipeCustomForVisualPipe(const QString code_location) : mafPipeVisual(code_location), m_PipeLine("Created") {
    m_Output = &m_Container;
}

void testVisualPipeCustomForVisualPipe::render() {
    
}

void testVisualPipeCustomForVisualPipe::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(QString::number(t));

    m_Container = new QString;
    m_Container.externalData()->append(m_PipeLine);
}


//------------------------------------------------------------------------------------------

void mafPipeVisualTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_VisualPipe = mafNEW(testVisualPipeCustomForVisualPipe);
}

void mafPipeVisualTest::cleanupTestCase() {
    mafDEL(m_VisualPipe);
    mafMessageHandler::instance()->shutdown();
    mafEventBus::mafEventBusManager::instance()->shutdown();
}

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

#include "mafPipeVisualTest.moc"
