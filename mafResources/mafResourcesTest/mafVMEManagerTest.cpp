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
#include <mafVMEManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafVMEManagerTest
 This class implements the test suite for mafVMEManager.
 */
class mafVMEManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        // Register all the creatable objects for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();
        m_EventBus = mafEventBusManager::instance();
        m_VMEManager = mafVMEManager::instance();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_VMEManager->shutdown();

        // Shutdown eventbus singleton and core singletons.
        m_EventBus->shutdown();
    }

    /// mafVMEManager allocation test case.
    void mafVMEManagerAllocationTest();
    /// VME managing test.
    void vmeManagingTest();

private:
    mafVMEManager *m_VMEManager;
    mafEventBusManager *m_EventBus;
};

void mafVMEManagerTest::mafVMEManagerAllocationTest() {
    QVERIFY(m_VMEManager != NULL);
}

void mafVMEManagerTest::vmeManagingTest() {
    mafCore::mafObjectBase *vme1 = mafNEWFromString("mafResources::mafVME");
    mafCore::mafObjectBase *vme2 = mafNEWFromString("mafResources::mafVME");

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme1));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme2));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    mafCore::mafObjectBase *sel_vme;
    mafGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
    m_EventBus->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

    QVERIFY(sel_vme == NULL);

    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme2));
    m_EventBus->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(sel_vme == vme2);

    mafDEL(vme1);
    mafDEL(vme2);
}

MAF_REGISTER_TEST(mafVMEManagerTest);
#include "mafVMEManagerTest.moc"

