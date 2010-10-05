/*
 *  mafClassFactoryTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafClassFactory.h>
#include <mafObject.h>

using namespace mafCore;

/**
 Class name: mafClassFactoryTest
 This class implements the test suite for mafClassFactory.
 */
class mafClassFactoryTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
    }

    /// create new object and check that is not NULL test case.
    void mafClassFactoryAllocationTest();

private:
    mafClassFactory<mafObject> m_ClassFactory; ///< Test var.
};

void mafClassFactoryTest::mafClassFactoryAllocationTest() {
    mafObjectBase *obj = m_ClassFactory.make(mafCodeLocation);
    QVERIFY(obj != NULL);
    mafDEL(obj);
}


MAF_REGISTER_TEST(mafClassFactoryTest);
#include "mafClassFactoryTest.moc"
