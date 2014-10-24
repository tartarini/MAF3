/*
 *  mafCoreSingletonsTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

#if defined(_WIN32) || defined(WIN32)
    #define TEST_LIBRARY_NAME "mafModuleTest.dll"
#else
    #ifdef __APPLE__
        #define TEST_LIBRARY_NAME "mafModuleTest.dylib"
    #else
        #define TEST_LIBRARY_NAME "mafModuleTest.so"
    #endif
#endif

using namespace mafCore;

void mafCoreSingletonsTest::initTestCase() {
    m_IdProvider = mafIdProvider::instance();
}

void mafCoreSingletonsTest::cleanupTestCase() {
}

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

#include "mafCoreSingletonsTest.moc"
