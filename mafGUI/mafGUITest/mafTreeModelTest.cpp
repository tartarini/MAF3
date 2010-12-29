/*
 *  mafTreeModelTest.cpp
 *  mafTreeModel
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>

#include <mafResourcesDefinitions.h>
#include <mafVME.h>

#include <mafGUIRegistration.h>
#include <QtGui>
#include <mafTreeModel.h>

using namespace mafCore;
using namespace mafGUI;
using namespace mafResources;

/**
 Class name: mafTreeModelTest
 This class implements the test suite for mafTreeModel.
 */
class mafTreeModelTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Register all the creatable objects for the mafGUI module.
        mafGUIRegistration::registerGUIObjects();
        m_TreeModel = new mafTreeModel();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_TreeModel;
    }

    /// allocation test case.
    void mafTreeModelAllocationTest();

    /// hierarchy set/get Test case.
    void hierarchyTest();

    /// inserting item test
    void insertNewItemTest();

    /// clearing the model test
    void clearTest();

private:
    mafTreeModel *m_TreeModel; ///< Reference to the tree item.
};

void mafTreeModelTest::mafTreeModelAllocationTest() {
    QVERIFY(m_TreeModel != NULL);
}


void mafTreeModelTest::hierarchyTest() {
    QVERIFY(m_TreeModel != NULL);

    mafHierarchy *m_HierarchyToManage = mafNEW(mafCore::mafHierarchy);

    mafVME* vmeRoot = mafNEW(mafResources::mafVME);
    vmeRoot->setObjectName("root");

    m_HierarchyToManage->addHierarchyNode(vmeRoot);

    mafVME* vmeChild0 = mafNEW(mafResources::mafVME);
    vmeChild0->setObjectName("vmeChild0");
    m_HierarchyToManage->addHierarchyNode(vmeChild0);

    mafVME* vmeChild0Child0 = mafNEW(mafResources::mafVME);
    vmeChild0Child0->setObjectName("vmeChild0Child0");
    m_HierarchyToManage->addHierarchyNode(vmeChild0Child0, vmeChild0);

    // Create a new child to add to the tree.
    mafVME* vmeChild1 = mafNEW(mafResources::mafVME);
    vmeChild1->setObjectName("vmeChild1");
    m_HierarchyToManage->addHierarchyNode(vmeChild1, vmeRoot);

    mafString info;
    m_HierarchyToManage->printInformation(info);
    mafMsgDebug() << info;
    /// end hierarchy creation

    m_TreeModel->setHierarchy(m_HierarchyToManage);


    mafVME* vmeChild1Child0 = mafNEW(mafResources::mafVME);
    vmeChild1Child0->setObjectName("vmeChild1Child0");
    m_HierarchyToManage->addHierarchyNode(vmeChild1Child0, vmeChild1); // maybe a signal is useful

    m_TreeModel->insertNewItem(mafTreeModel::AsChild,vmeChild1Child0, m_TreeModel->currentIndex());

    mafDEL(m_HierarchyToManage);
    mafDEL(vmeChild1Child0);
    mafDEL(vmeChild1);
    mafDEL(vmeChild0Child0);
    mafDEL(vmeChild0);
    mafDEL(vmeRoot);
}

void mafTreeModelTest::insertNewItemTest() {

}


void mafTreeModelTest::clearTest() {    
    m_TreeModel->clear();
}


MAF_REGISTER_TEST(mafTreeModelTest);
#include "mafTreeModelTest.moc"

