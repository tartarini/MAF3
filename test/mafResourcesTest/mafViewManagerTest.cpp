/*
 *  mafViewManagerTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2011 B3C. All rights reserved.
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
#include <mafMemento.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: testViewManagerObserver
 This class implements the observer for the signals emitted by the mafViewManager.
 */
class testViewManagerObserver : public QObject {
    Q_OBJECT

public:
    /// object constructor
    testViewManagerObserver();

    /// Return the current value of m_View
    mafObjectBase *view();

public Q_SLOTS:
    /// View Created slot
    void viewCreatedSlot(mafCore::mafObjectBase *view);

private:
    mafObjectBase *m_View; ///< Reference to the view object.
};

testViewManagerObserver::testViewManagerObserver() : m_View(NULL) {
    mafRegisterLocalCallback("maf.local.resources.view.created", this, "viewCreatedSlot(mafCore::mafObjectBase *)");
}

mafObjectBase *testViewManagerObserver::view() {
    return m_View;
}

void testViewManagerObserver::viewCreatedSlot(mafCore::mafObjectBase *view) {
    m_View = view;
}

/**
 Class name: testViewManagerObserver
 This class implements the observer for the signals emitted by the mafViewManager.
 */
class testViewCustom : public mafView {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    testViewCustom(const QString code_location = "");
    
    /// Crete view.
    /*virtual*/ bool initialize();
};

testViewCustom::testViewCustom(const QString code_location) : mafView(code_location) {
    
}

bool testViewCustom::initialize() {
    Superclass::setupSceneGraph();
    return true;
}


/**
 Class name: mafViewManagerTest
 This class implements the test suite for mafViewManager.
 */
class mafViewManagerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        // Register all the objects that can be instantiated for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();

        m_Observer = new testViewManagerObserver();

        m_EventBus = mafEventBusManager::instance();
        m_ViewManager = mafViewManager::instance();
        
        mafRegisterObject(testViewCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafUnregisterObject(testViewCustom);
        m_ViewManager->shutdown();

        delete m_Observer;

        // Shutdown event-bus singleton and core singletons.
        m_EventBus->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

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
    mafViewManager *m_ViewManager; ///< View Manager test variable
    mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    testViewManagerObserver *m_Observer; ///< test observer for view created and selected.
    QString m_Hash1;
    QString m_Hash2;
};

void mafViewManagerTest::mafViewManagerAllocationTest() {
    QVERIFY(m_ViewManager != NULL);
}

void mafViewManagerTest::createViewTest() {
    // Create a new view from its string type name.
    QString vt("testViewCustom");
    QString vn("View test");

    // Send the message through the vent bus to create it.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, vt));
    argList.append(mafEventArgument(QString, vn));
    m_EventBus->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList);

    mafObjectBase *obj = m_Observer->view();
    m_Hash1 = obj->objectHash();
    QVERIFY(obj != NULL);

    // Create another view...
    m_EventBus->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList);
    obj = m_Observer->view();
    QVERIFY(obj != NULL);
    m_Hash2 = obj->objectHash();
    QVERIFY(m_Hash1 != m_Hash2);
}

void mafViewManagerTest::selectViewTest() {
    mafObjectBase *obj = mafObjectRegistry::instance()->objectFromHash(m_Hash1);
    QVERIFY(m_Observer->view() != obj);

    mafObjectBase *obj2 = mafObjectRegistry::instance()->objectFromHash(m_Hash2);
    QVERIFY(m_Observer->view() == obj2);

    // Ask the view manager to select the previous created view.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
    m_EventBus->notifyEvent("maf.local.resources.view.select", mafEventTypeLocal, &argList);
    
    mafObjectBase *selectedView;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, selectedView);   
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &ret_val);

    QVERIFY(selectedView != NULL);
    QVERIFY(selectedView == obj);
}

void mafViewManagerTest::mementoViewTest() {
    // Should contains 1 view.
    mafMemento *m = m_ViewManager->createMemento();

    // Shutdown the manager => will destroy all view and will reset the created view list.
    m_ViewManager->shutdown();

    // Check that all views should be NULL.
    mafObjectBase *obj1 = mafObjectRegistry::instance()->objectFromHash(m_Hash1);
    mafObjectBase *obj2 = mafObjectRegistry::instance()->objectFromHash(m_Hash2);

    QVERIFY(obj1 == NULL);
    QVERIFY(obj2 == NULL);

    // Restore the previous view manager state.
    m_ViewManager->setMemento(m);

    // Retrieve the selected view that should be not NULL.
    QVERIFY(m_Observer->view() != NULL);

    mafDEL(m);
}

void mafViewManagerTest::removeAndDestructionTest() {
    mafObjectBase *obj = m_Observer->view();
    QString hash1 = obj->objectHash();
    mafDEL(obj);
    
    mafObjectBase *selectedView;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, selectedView);   
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &ret_val);
    
    QString hash2 = selectedView->objectHash();
    QVERIFY(hash1 != hash2);
}

MAF_REGISTER_TEST(mafViewManagerTest);
#include "mafViewManagerTest.moc"

