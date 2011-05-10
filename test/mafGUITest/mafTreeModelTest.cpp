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

    /// hierarchy set/get Test case and insert Item in model.
    void hierarchyAndInsertItemTest();

    /// clearing the model test
    void clearTest();

private:
    mafTreeModel *m_TreeModel; ///< Reference to the tree item.
};

void mafTreeModelTest::mafTreeModelAllocationTest() {
    QVERIFY(m_TreeModel != NULL);
}


void mafTreeModelTest::hierarchyAndInsertItemTest() {
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

    QString info;
    m_HierarchyToManage->printInformation(info);
    qDebug() << info;
    /// end hierarchy creation

    m_TreeModel->setHierarchy(m_HierarchyToManage);
    int rows = m_TreeModel->rowCount(m_TreeModel->indexFromData(vmeRoot));
    QVERIFY(rows == 2);

    mafVME* vmeChild2 = mafNEW(mafResources::mafVME);
    vmeChild2->setObjectName("vmeChild2");
    m_HierarchyToManage->addHierarchyNode(vmeChild2, vmeRoot); // maybe a signal is useful

//    m_TreeModel->insertNewItem(mafTreeModel::AsChild,vmeChild2, m_TreeModel->currentIndex()); //currentIndex is in root
    rows = m_TreeModel->rowCount(m_TreeModel->indexFromData(vmeRoot));
    QVERIFY(rows == 3);

    // remove element
    m_TreeModel->itemDetached(vmeRoot); // trying to remove root, impossible
    rows = m_TreeModel->rowCount(m_TreeModel->indexFromData(vmeRoot));
    QVERIFY(rows == 3);

    m_TreeModel->itemDetached(vmeChild0);
    rows = m_TreeModel->rowCount(m_TreeModel->indexFromData(vmeRoot));
    QVERIFY(rows == 2);

    m_HierarchyToManage->clear();

    mafDEL(m_HierarchyToManage);
    mafDEL(vmeChild2);
    mafDEL(vmeChild1);
    mafDEL(vmeChild0Child0);
    mafDEL(vmeChild0);
    mafDEL(vmeRoot);
}


void mafTreeModelTest::clearTest() {    
    m_TreeModel->clearModel();
}


MAF_REGISTER_TEST(mafTreeModelTest);
#include "mafTreeModelTest.moc"

