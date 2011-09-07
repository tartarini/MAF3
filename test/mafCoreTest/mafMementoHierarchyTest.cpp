/*
 *  mafMementoHierarchyTest.cpp
 *  mafCoreTest
 *
 *  Created by Roberto Mucci on 24/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafHierarchy.h>
#include <mafMementoHierarchy.h>

using namespace mafCore;

/**
 Class name: mafMementoHierarchyTest
 This class implements the test suite for mafMementoHierarchy.
 */
class mafMementoHierarchyTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        // Create before the instance of the Serialization manager, which will register signals.

        mafMessageHandler::instance()->installMessageHandler();
}

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafMessageHandler::instance()->shutdown();
    }

    /// mafMementoHierarchy allocation test case.
    void mafMementoHierarchyDefaultAllocationTest();

    /// mafMementoHierarchy traverse a more complex hierarchy
    void mafMementoHierarchyTraverseTreeTest();

    
private:
};

void mafMementoHierarchyTest::mafMementoHierarchyDefaultAllocationTest() {
    mafHierarchy *h = mafNEW(mafCore::mafHierarchy);
    h->setObjectName("Test memento hierarchy");
    mafObject *obj = mafNEW(mafCore::mafObject);
    h->addHierarchyNode(obj);
    //! <snippet>
    mafMementoHierarchy *memento = new mafMementoHierarchy(h, mafCodeLocation);
    //! </snippet>
    QVERIFY(memento != NULL);
    mafDEL(memento);
    mafDEL(h);
    mafDEL(obj);
}

void mafMementoHierarchyTest::mafMementoHierarchyTraverseTreeTest() {
    mafHierarchy *h = mafNEW(mafCore::mafHierarchy);
    h->setObjectName("Test memento hierarchy");
    mafObject *objA = mafNEW(mafCore::mafObject);
    mafObject *objAA = mafNEW(mafCore::mafObject);
    mafObject *objAB = mafNEW(mafCore::mafObject);
    mafObject *objABA = mafNEW(mafCore::mafObject);
    
    h->addHierarchyNode(objA);
    h->addHierarchyNode(objAA, objA);
    h->addHierarchyNode(objAB, objA);
    h->addHierarchyNode(objABA, objAB);
    mafDEL(objA);
    mafDEL(objAA);
    mafDEL(objAB);
    mafDEL(objABA);
    
    mafObject *objB = mafNEW(mafCore::mafObject);
    mafObject *objBA = mafNEW(mafCore::mafObject);
    mafObject *objBB = mafNEW(mafCore::mafObject);
    mafObject *objBC = mafNEW(mafCore::mafObject);
    mafObject *objBD = mafNEW(mafCore::mafObject);
    
    h->addHierarchyNode(objB);
    h->addHierarchyNode(objBA);
    h->addHierarchyNode(objBB);
    h->addHierarchyNode(objBC);
    h->addHierarchyNode(objBD);
    mafDEL(objB);
    mafDEL(objBA);
    mafDEL(objBB);
    mafDEL(objBC);
    mafDEL(objBD);

    //! <snippet>
    mafMementoHierarchy *memento = new mafMementoHierarchy(h, mafCodeLocation);
    //! </snippet>
    QVERIFY(memento != NULL);
    mafDEL(memento);
    mafDEL(h);
}


MAF_REGISTER_TEST(mafMementoHierarchyTest);
#include "mafMementoHierarchyTest.moc"
