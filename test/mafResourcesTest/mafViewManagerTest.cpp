/*
 *  mafViewManagerTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

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

protected:
    /// Object destructor.
    /* virtual */ ~testViewCustom();
};

testViewCustom::testViewCustom(const QString code_location) : mafView(code_location) {
    
}

testViewCustom::~testViewCustom() {
    mafDEL(m_ToolHandler);
}

bool testViewCustom::initialize() {
    Superclass::setupSceneGraph();
    return true;
}


void mafViewManagerTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    // Register all the objects that can be instantiated for the mafResources module.
    mafResourcesRegistration::registerResourcesObjects();

    m_Observer = new testViewManagerObserver();

    m_EventBus = mafEventBusManager::instance();
    m_ViewManager = mafViewManager::instance();
    
    mafRegisterObject(testViewCustom);
}

/// Cleanup test variables memory allocation.
void mafViewManagerTest::cleanupTestCase() {
    m_ViewManager->shutdown();

    delete m_Observer;

    mafUnregisterObject(testViewCustom);

    // Shutdown event-bus singleton and core singletons.
    m_EventBus->shutdown();
    mafMessageHandler::instance()->shutdown();
}

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

#include "mafViewManagerTest.moc"

