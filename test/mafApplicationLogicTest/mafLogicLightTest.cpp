/*
 *  mafLogicLightTest.cpp
 *  mafLogicTest
 *
 *  Created by Paolo Quadrani on 21/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafLogicLight.h>
#include <mafCoreSingletons.h>

using namespace mafCore;
using namespace mafApplicationLogic;

/**
 Class name: mafLogicLightTest
 This class implements the test suite for mafLogicLight.
 */
 //! <title>
//mafLogicLight
//! </title>
//! <description>
//This class define the basic implementation of the framework logic.
//It allows simply to instantiate the event bus, register the core objects
//into the registry and on destruction allows to clean upo the registration
//of the objects and shutdown the event bus.
//This logic clas can be used with those applications that needs only the
//mafCore and mafEventBus module whitout the need of the mafResources module.
//! </description>

class mafLogicLightTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_LogicLight = mafNEW(mafApplicationLogic::mafLogicLight);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_LogicLight);
    }

    /// Check the existence of the mafLogicLight allocation.
    void mafLogicLightAllocationTest();

    /// mafLogicLight initialization test
    void initializeTest();

private:
    mafLogicLight *m_LogicLight; ///< Test variable.
};

void mafLogicLightTest::mafLogicLightAllocationTest() {
    QVERIFY(m_LogicLight != NULL);
}

void mafLogicLightTest::initializeTest() {
    bool result = m_LogicLight->initialize();
    QVERIFY(result);
}

MAF_REGISTER_TEST(mafLogicLightTest);
#include "mafLogicLightTest.moc"

