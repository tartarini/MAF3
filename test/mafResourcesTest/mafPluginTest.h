/*
 *  mafPluginTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafResourcesSingletons.h>
#include <mafEventBusManager.h>
#include <mafPlugin.h>

/**
 Class name: mafPluginTest
 This class implements the test suite for mafPlugin.
 */
class mafPluginTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafPlugin allocation test case.
    void mafPluginAllocationTest();
    /// Plug in Registration and info
    void pluginRegistrationTest();

private:
    mafResources::mafPlugin *m_Plugin; ///< Test var.
    QString m_PluginName; ///< Test var.
};
