/*
 *  mafVMETest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafContainer.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


/**
 Class name: mafVMETest
 This class implements the test suite for mafVME.
 */

 //! <title>
//mafVME
//! </title>
//! <description>
//mafVME defines the base class for the VMEs in MAF3.
//! </description>

class mafVMETest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
    }

    /// mafVME allocation test case.
    void mafVMEAllocationTest();

private:
    mafVME *m_VME; ///< Test var.
};

void mafVMETest::mafVMEAllocationTest() {
    QVERIFY(m_VME != NULL);
    mafDataSetCollection *collection = m_VME->dataSetCollection();
    QVERIFY(collection != NULL);
}

MAF_REGISTER_TEST(mafVMETest);
#include "mafVMETest.moc"
