/*
 *  mafUILoaderQtTest.h
 *  mafGUITest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestConfig.h>
#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafProxy.h>
#include <mafEventBusManager.h>
#include <mafUILoaderQt.h>

//forward classes
class testUILoaderRequestor;

/**
 Class name: mafUILoaderQtTest
 This class implements the test suite for mafUILoaderQt.
 */
class mafUILoaderQtTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafUILoader allocation test case.
    void mafUILoaderQtAllocationTest();

    /// mafUILoader allocation test case.
    void mafUILoaderQtUILoadTest();

private:
    mafGUI::mafUILoaderQt *m_UILoader; ///< Reference to the UI Loader.
    testUILoaderRequestor *m_LoadRequestor; ///< Variable containing the UI load requestor.
};
