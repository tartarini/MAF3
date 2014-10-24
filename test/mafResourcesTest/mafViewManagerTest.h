/*
 *  mafViewManagerTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafEventBusManager.h>
#include <mafViewManager.h>
#include <mafView.h>
#include <mafToolHandler.h>
#include <mafMemento.h>

//forward declariations
class testViewManagerObserver;
class testViewCustom;

/**
 Class name: mafViewManagerTest
 This class implements the test suite for mafViewManager.
 */
class mafViewManagerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafViewManager allocation test case.
    void mafViewManagerAllocationTest();
    /// Test for createView.
    void createViewTest();
    /// Test select View method
    void selectViewTest();
    /// Test memento (for store and retrieve settings)
    void mementoViewTest();
    /// Test the view remove and destruction
    void removeAndDestructionTest();

private:
    mafResources::mafViewManager *m_ViewManager; ///< View Manager test variable
    mafEventBus::mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    testViewManagerObserver *m_Observer; ///< test observer for view created and selected.
    QString m_Hash1;
    QString m_Hash2;
};
