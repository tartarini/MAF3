/*
 *  mafCoreSingletonsTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>

#ifdef WIN32
    #define TEST_LIBRARY_NAME "mafModuleTest.dll"
#else
    #ifdef __APPLE__
        #define TEST_LIBRARY_NAME "mafModuleTest.dylib"
    #else
        #define TEST_LIBRARY_NAME "mafModuleTest.so"
    #endif
#endif

using namespace mafCore;

/**
 Class name: mafCoreSingletonsTest
 This class implements the test suite for mafCoreSingletons.
 */
class mafCoreSingletonsTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_IdProvider = mafIdProvider::instance();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
    }

    /// Call the initialization for mafCoreSingletons and verify that are initialized.
    void mafSingletonsInitializeTest();

private:
    mafIdProvider *m_IdProvider; ///< Test var.
};

void mafCoreSingletonsTest::mafSingletonsInitializeTest() {
    QVERIFY(m_IdProvider != NULL);

    //mafInitializeModule test
    QLibrary *module_initialized;
    module_initialized = mafInitializeModule(TEST_LIBRARY_NAME);
    QVERIFY(module_initialized != NULL);

    //mafShutdownModule test
    bool result = false;
    result = mafShutdownModule(module_initialized);
    QVERIFY(result);
}


MAF_REGISTER_TEST(mafCoreSingletonsTest);
#include "mafCoreSingletonsTest.moc"
