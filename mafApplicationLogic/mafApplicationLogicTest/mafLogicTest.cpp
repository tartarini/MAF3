/*
 *  mafLogicTest.cpp
 *  mafLogicTest
 *
 *  Created by Paolo Quadrani on 12/11/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafLogic.h>
#include <mafCoreSingletons.h>

using namespace mafCore;
using namespace mafApplicationLogic;

/**
 Class name: mafLogicTest
 This class implements the test suite for mafLogic.
 */
class mafLogicTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafCoreSingletons::mafSingletonsInitialize();
        m_Logic = mafNEW(mafApplicationLogic::mafLogic);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Logic);
        mafCoreSingletons::mafSingletonsShutdown();
    }

    /// Check the existence of the mafLogic allocation.
    void mafLogicAllocationTest();

    /// Test the initialization of the mafLogic
    void initializeTest();

    /// Test the working directory accessors.
    void workingDirectoryTest();

private:
    mafLogic *m_Logic; ///< Test variable.
};

void mafLogicTest::mafLogicAllocationTest() {
    QVERIFY(m_Logic != NULL);
}

void mafLogicTest::initializeTest() {
    bool result = m_Logic->initialize();
    QVERIFY(result);
}

void mafLogicTest::workingDirectoryTest() {
    mafString res = QDir::toNativeSeparators("/myBaseDir/myDir1/myDir2/");
    m_Logic->setWorkingDirectory("/myBaseDir/myDir1/myDir2/");
    mafString wd = m_Logic->workingDirectory();
    QCOMPARE(wd, res);
    m_Logic->setWorkingDirectory("\\myBaseDir\\myDir1\\myDir2\\");
    wd = m_Logic->workingDirectory();
    QCOMPARE(wd, res);
    m_Logic->setWorkingDirectory("/myBaseDir\\myDir1\\myDir2/");
    wd = m_Logic->workingDirectory();
    QCOMPARE(wd, res);
    m_Logic->setWorkingDirectory("/myBaseDir/myDir1");
    wd = m_Logic->workingDirectory();
    QVERIFY(res != wd);
}

MAF_REGISTER_TEST(mafLogicTest);
#include "mafLogicTest.moc"
