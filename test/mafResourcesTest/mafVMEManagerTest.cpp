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
#include <mafVMEManager.h>
#include <mafMatrix.h>
#include <mafVME.h>
#include <mafDataSet.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafVMEManagerTest
 This class implements the test suite for mafVMEManager.
 */
class mafVMEManagerTest : public QObject {
    Q_OBJECT

/// create a huge vme tree for benchmark.
void unbalancedTreeRandomCreation(unsigned int numberOfElements); 
    
private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        // Register all the objects into the factory for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();
        m_EventBus = mafEventBusManager::instance();
        m_VMEManager = mafVMEManager::instance();

        //Request hierarchy
        mafHierarchyPointer hierarchy;
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);

        //Select root
        mafObject *root;
        ret_val = mafEventReturnArgument(mafCore::mafObject *, root);
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.root", mafEventTypeLocal, NULL, &ret_val);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_VMEManager->shutdown();

        //restore vme manager status
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal);
        
        // Shutdown event bus singleton and core singletons.
        m_EventBus->shutdown();
        mafMessageHandler::instance()->shutdown();
    }
    
    /// mafVMEManager allocation test case.
    void mafVMEManagerAllocationTest();
    /// VME managing test.
    void vmeManagingTest();
    /// create hierarchy Test
    void createHierarchyTest();
    /// absolute pose matrix Test
    void absolutePoseMatrixTest();
    /// benchmarking the absolute matrix
    void benchmarkarkedAbsoluteMatrixTest();

private:
    mafVMEManager *m_VMEManager;
    mafEventBusManager *m_EventBus;
    mafHierarchy *m_Hierarchy; ///< Test var.
    mafObjectBase *m_SelectedVME;
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
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
    m_EventBus->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

    QVERIFY(sel_vme != NULL); // root is selected

    mafDEL(vme1);
    mafDEL(vme2);
}

void mafVMEManagerTest::createHierarchyTest() {
    // this test is a smke test for verifying that manager doesn't crash.
    // createHierarchy is a private method
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal);
}

void mafVMEManagerTest::absolutePoseMatrixTest() {
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal);
    
    mafVME *vme1 = mafNEW(mafResources::mafVME);
    mafVME *vme2 = mafNEW(mafResources::mafVME);
    
    mafDataSet *dataVME2 = mafNEW(mafResources::mafDataSet);
    
    mafMatrix first;
    first.setElement(0, 0, 2.0); first.setElement(0, 1, 2.0); first.setElement(0, 2, 0.0); first.setElement(0, 3, 2.0);
    first.setElement(1, 0, 2.0); first.setElement(1, 1, 2.0); first.setElement(1, 2, 2.0); first.setElement(1, 3, 2.0);
    first.setElement(2, 0, 0.0); first.setElement(2, 1, 2.0); first.setElement(2, 2, 2.0); first.setElement(2, 3, 2.0);
    first.setElement(3, 0, 2.0); first.setElement(3, 1, 0.0); first.setElement(3, 2, 2.0); first.setElement(3, 3, 2.0);
    
    dataVME2->setPoseMatrix(&first);
    vme2->dataSetCollection()->insertItem(dataVME2);
    dataVME2->release();
    
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme1));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme2));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    
    m_EventBus->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal,  &argList);

    mafVME *vme3 = mafNEW(mafResources::mafVME);
    mafDataSet *dataVME3 = mafNEW(mafResources::mafDataSet);
    
    mafMatrix second;
    second.setElement(0, 0, 0.0); second.setElement(0, 1, 0.5); second.setElement(0, 2, -0.5);second.setElement(0, 3, 0.0);
    second.setElement(1, 0, 0.0); second.setElement(1, 1, 0.5); second.setElement(1, 2, 0.0); second.setElement(1, 3, -0.5);
    second.setElement(2, 0, -0.5);second.setElement(2, 1, 0.5); second.setElement(2, 2, 0.0); second.setElement(2, 3, 0.0);
    second.setElement(3, 0, 0.5); second.setElement(3, 1, -1.0);second.setElement(3, 2, 0.5); second.setElement(3, 3, 0.5);
    
    dataVME3->setPoseMatrix(&second);
    vme3->dataSetCollection()->insertItem(dataVME3);
    dataVME3->release();
    
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme3));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    //   root 
    //      |__ vme1 ...
    //      |__ vme2 (mat A)
    //             |__vme3 (mat inv(A))
    // absolute pose vme3 = identity
    
    mafMatrix identity;
    identity.setElement(0, 0, 1.0); identity.setElement(0, 1, 0.0); identity.setElement(0, 2, 0.0); identity.setElement(0, 3, 0.0);
    identity.setElement(1, 0, 0.0); identity.setElement(1, 1, 1.0); identity.setElement(1, 2, 0.0); identity.setElement(1, 3, 0.0);
    identity.setElement(2, 0, 0.0); identity.setElement(2, 1, 0.0); identity.setElement(2, 2, 1.0); identity.setElement(2, 3, 0.0);
    identity.setElement(3, 0, 0.0); identity.setElement(3, 1, 0.0); identity.setElement(3, 2, 0.0); identity.setElement(3, 3, 1.0);
    
    mafMatrixPointer absMatrix = NULL;
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme3));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrixPointer, absMatrix);
    m_EventBus->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);
    
    absMatrix->description();
    QVERIFY(identity.isEqual(*absMatrix));
    delete absMatrix;
    
    mafDEL(vme1);
    mafDEL(vme2);
    mafDEL(vme3);
    
    //create new big tree for the next test
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal);
    unbalancedTreeRandomCreation(200);
}

void mafVMEManagerTest::benchmarkarkedAbsoluteMatrixTest() {
    QBENCHMARK {
        mafMatrixPointer absMatrix = NULL;
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, m_SelectedVME));
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrixPointer, absMatrix);
        
        m_EventBus->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);
        delete absMatrix;
    }
}

void mafVMEManagerTest::unbalancedTreeRandomCreation(unsigned int numberOfElements) {
    REQUIRE(numberOfElements != 0);

    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, m_Hierarchy);
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL , &ret_val);
    
    unsigned int count = 0;
    while(count != numberOfElements) {
        mafVME *vme = mafNEW(mafResources::mafVME);
        
        mafDataSet *dataVME = mafNEW(mafResources::mafDataSet);
        
        mafMatrix first;
        first.setElement(0, 0, 2.0); first.setElement(0, 1, 2.0); first.setElement(0, 2, 0.0); first.setElement(0, 3, 2.0);
        first.setElement(1, 0, 2.0); first.setElement(1, 1, 2.0); first.setElement(1, 2, 2.0); first.setElement(1, 3, 2.0);
        first.setElement(2, 0, 0.0); first.setElement(2, 1, 2.0); first.setElement(2, 2, 2.0); first.setElement(2, 3, 2.0);
        first.setElement(3, 0, 2.0); first.setElement(3, 1, 0.0); first.setElement(3, 2, 2.0); first.setElement(3, 3, 2.0);
        
        //first.setIdentity();
        
        dataVME->setPoseMatrix(&first);
        vme->dataSetCollection()->insertItem(dataVME);
        dataVME->release();
        
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
        m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
        vme->release();
        m_EventBus->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal,  &argList); //select the last added
        m_SelectedVME = vme;
        
        ++count;
    }
}

MAF_REGISTER_TEST(mafVMEManagerTest);
#include "mafVMEManagerTest.moc"

