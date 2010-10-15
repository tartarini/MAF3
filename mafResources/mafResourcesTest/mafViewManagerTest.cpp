/*
 *  mafViewManagerTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafEventBusManager.h>
#include <mafViewManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafViewManagerTest
 This class implements the test suite for mafViewManager.
 */
class mafViewManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Register all the creatable objects for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();

        m_View = NULL;

        m_EventBus = mafEventBusManager::instance();
        m_ViewManager = mafViewManager::instance();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_ViewManager->shutdown();

        // Shutdown eventbus singleton and core singletons.
        m_EventBus->shutdown();
    }

    /// mafViewManager allocation test case.
    void mafViewManagerAllocationTest();
    /// Test for createView.
    void createViewTest();
    /// Test select View method
    void selectViewTest();
    /// Test memento (for store and retriev settings)
    void mementoViewTest();


private:
    mafViewManager *m_ViewManager; ///< View Manager test variable
    mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    mafObjectBase *m_View; ///< Reference to the view object.
};

void mafViewManagerTest::mafViewManagerAllocationTest() {
    QVERIFY(m_ViewManager != NULL);
}

void mafViewManagerTest::createViewTest() {
    // Create a new view from its string type name.
    mafString vt("mafResources::mafView");

    // Send the message through the vent bus to create it.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, vt));
    m_EventBus->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList, &mafEventReturnArgument(mafCore::mafObjectBase *, m_View));

    // Return value should be not NULL.
    QVERIFY(m_View != NULL);

    // Create another view...
    mafObjectBase *second_view;
    m_EventBus->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList, &mafEventReturnArgument(mafCore::mafObjectBase *, second_view));
    QVERIFY(second_view != NULL);
}

void mafViewManagerTest::selectViewTest() {
    // Ask for default selected view.
    mafCore::mafObjectBase *sel_view;
    m_EventBus->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &mafEventReturnArgument(mafCore::mafObjectBase *, sel_view));

    QVERIFY(sel_view != m_View);

    // Ask the view manager to select the previous created view.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_View));
    m_EventBus->notifyEvent("maf.local.resources.view.select", mafEventTypeLocal, &argList);

    // Retrieve the selected view that now should be not NULL and equal to the previous created view.
    m_EventBus->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &mafEventReturnArgument(mafCore::mafObjectBase *, sel_view));
    QVERIFY(sel_view != NULL);
    QVERIFY(sel_view == m_View);
}

void mafViewManagerTest::mementoViewTest() {
    // Should contains 1 view.
    mafMemento *m = m_ViewManager->createMemento();

    // Shutdown the manager => will destroy all view and will reset the created view list.
    m_ViewManager->shutdown();
    // Retrieve the selected view that should be NULL.
    mafCore::mafObjectBase *sel_view;
    m_EventBus->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &mafEventReturnArgument(mafCore::mafObjectBase *, sel_view));
    QVERIFY(sel_view == NULL);

    // Restore the previous view manager state.
    m_ViewManager->setMemento(m);
    // Retrieve the selected view that should be not NULL.
    m_EventBus->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &mafEventReturnArgument(mafCore::mafObjectBase *, sel_view));
    QVERIFY(sel_view != NULL);

    mafDEL(m);
}

MAF_REGISTER_TEST(mafViewManagerTest);
#include "mafViewManagerTest.moc"

