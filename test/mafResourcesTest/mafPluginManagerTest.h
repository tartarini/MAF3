/*
 *  mafPluginManagerTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafEventBusManager.h>
#include <mafPluginManager.h>
#include <mafPipeData.h>
#include <mafPlugin.h>

//forward declarations
class testPluginObserver;

/**
 Class name: mafPluginManagerTest
 This class implements the test suite for mafPluginManager.
 */

//! <title>
//mafPluginManager
//! </title>
//! <description>
//mafPluginManager provides the engine for loading plug-ins and define
//the ID: REGISTER_PLUGIN used by external libraries to register their plugged objects.
//! </description>

class mafPluginManagerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafPluginManager allocation test case.
    void mafPluginManagerAllocationTest();
    /// Test loading plug-in compiled as dynamic library
    void mafPluginManagerLoadPluginTest();
    /// Test loading plug-in compiled as dynamic library
    void mafPluginManagerLoadQtPluginTest();

private:
    mafResources::mafPluginManager *m_PluginManager; ///< Test var.
    QString m_PluginName; ///< Test var.
    QString m_QtPluginName; ///< Test var.
    testPluginObserver *m_Observer; ///< Test observer.
};
