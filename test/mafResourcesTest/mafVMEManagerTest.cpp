/*
 *  mafViewManagerTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

void mafVMEManagerTest::initTestCase() {
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
void mafVMEManagerTest::cleanupTestCase() {
    m_VMEManager->shutdown();

    //restore vme manager status
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal);
    
    // Shutdown event bus singleton and core singletons.
    m_EventBus->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafVMEManagerTest::mafVMEManagerAllocationTest() {
    QVERIFY(m_VMEManager != NULL);
}

void mafVMEManagerTest::vmeManagingTest() {
    mafResources::mafVME *vme1 =  mafNEW(mafResources::mafVME);
    mafResources::mafVME *vme2 =  mafNEW(mafResources::mafVME);
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafResources::mafVME *, vme1));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    argList.clear();
    argList.append(mafEventArgument(mafResources::mafVME *, vme2));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    mafResources::mafVME *sel_vme;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafVME *, sel_vme);
    m_EventBus->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(sel_vme != NULL); // root is selected

    mafDEL(vme1);
    mafDEL(vme2);
}

void mafVMEManagerTest::createHierarchyTest() {
    // this test is a smoke test for verifying that manager doesn't crash.
    // createHierarchy is a private method
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal);
}

void mafVMEManagerTest::absolutePoseMatrixTest() {
    m_EventBus->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal);
    
    mafVME *vme1 = mafNEW(mafResources::mafVME);
    mafVME *vme2 = mafNEW(mafResources::mafVME);
    
    mafDataSet *dataVME2 = mafNEW(mafResources::mafDataSet);
    
    mafMatrix4x4 first;
    first(0,0) = 2.0; first(0,1) = 2.0; first(0,2) = 0.0; first(0,3) = 2.0;
    first(1,0) = 2.0; first(1,1) = 2.0; first(1,2) = 2.0; first(1,3) = 2.0;
    first(2,0) = 0.0; first(2,1) = 2.0; first(2,2) = 2.0; first(2,3) = 2.0;
    first(3,0) = 2.0; first(3,1) = 0.0; first(3,2) = 2.0; first(3,3) = 2.0;
    
    dataVME2->setPoseMatrix(&first);
    vme2->dataSetCollection()->insertItem(dataVME2);
    dataVME2->release();
    
    mafEventArgumentsList argList;
    mafCore::mafObjectBase *send = vme1;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, send));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    argList.clear();
    send = vme2;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, send));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    m_EventBus->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal,  &argList);

    mafVME *vme3 = mafNEW(mafResources::mafVME);
    mafDataSet *dataVME3 = mafNEW(mafResources::mafDataSet);
    
    mafMatrix4x4 second;	
	second(0,0) = 0.0;  second(0,1) = 0.5;  second(0,2) = -0.5; second(0,3) = 0.0;
	second(1,0) = 0.0;  second(1,1) = 0.5;  second(1,2) = 0.0;  second(1,3) = -0.5;
	second(2,0) = -0.5; second(2,1) = 0.5;  second(2,2) = 0.0;  second(2,3) = 0.0;
	second(3,0) = 0.5;  second(3,1) = -1.0; second(3,2) = 0.5;  second(3,3) = 0.5;
    
    dataVME3->setPoseMatrix(&second);
    vme3->dataSetCollection()->insertItem(dataVME3);
    dataVME3->release();
    
    argList.clear();
    send = vme3;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, send));
    m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    //   root 
    //      |__ vme1 ...
    //      |__ vme2 (mat A)
    //             |__vme3 (mat inv(A))
    // absolute pose vme3 = identity
    
    mafMatrix4x4 identity;
    identity.setToIdentity();
    
    mafMatrix4x4Pointer absMatrix = NULL;
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, send));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrix4x4Pointer, absMatrix);
    m_EventBus->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);
    
    //absMatrix->description();
    QVERIFY(identity == *absMatrix);
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
        mafMatrix4x4Pointer absMatrix = NULL;
        mafEventArgumentsList argList;
        argList.append(mafEventArgument( mafCore::mafObjectBase *, m_SelectedVME));
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrix4x4Pointer, absMatrix);
        
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
        
        mafMatrix4x4 first;

		first(0,0) = 2.0; first(0,1) = 2.0; first(0,2) = 0.0; first(0,3) = 2.0;
		first(1,0) = 2.0; first(1,1) = 2.0; first(1,2) = 2.0; first(1,3) = 2.0;
		first(2,0) = 0.0; first(2,1) = 2.0; first(2,2) = 2.0; first(2,3) = 2.0;
		first(3,0) = 2.0; first(3,1) = 0.0; first(3,2) = 2.0; first(3,3) = 2.0;


        //first.setToIdentity();
        
        dataVME->setPoseMatrix(&first);
        vme->dataSetCollection()->insertItem(dataVME);
        dataVME->release();
        
        mafEventArgumentsList argList;
        argList.append(mafEventArgument( mafCore::mafObjectBase *, vme));
        m_EventBus->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
        vme->release();
        m_EventBus->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal,  &argList); //select the last added
        m_SelectedVME = vme;
        
        ++count;
    }
}

#include "mafVMEManagerTest.moc"

