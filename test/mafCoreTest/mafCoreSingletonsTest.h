/*
 *  mafCoreSingletonsTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>

/**
 Class name: mafCoreSingletonsTest
 This class implements the test suite for mafCoreSingletons.
 */
class mafCoreSingletonsTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();
    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// Call the initialization for mafCoreSingletons and verify that are initialized.
    void mafSingletonsInitializeTest();

private:
    mafIdProvider *m_IdProvider; ///< Test var.
};
