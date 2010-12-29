/*
 *  mafTreeWidgetTest.cpp
 *  mafTreeWidget
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafHierarchy.h>

#include <mafResourcesDefinitions.h>
#include <mafVME.h>

#include <mafGUIRegistration.h>
#include <QtGui>
#include <mafTreeWidget.h>
#include <mafTreeModel.h>



using namespace mafCore;
using namespace mafGUI;
using namespace mafResources;

/**
 Class name: mafTreeWidgetTest
 This class implements the test suite for mafTreeWidget.
 */
class mafTreeWidgetTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Register all the creatable objects for the mafGUI module.
        mafGUIRegistration::registerGUIObjects();
        m_TreeWidget = new mafTreeWidget();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {

    }

    /// allocation test case.
    void mafTreeWidgetAllocationTest();

private:
    mafTreeWidget *m_TreeWidget; ///< Reference to the Tree Widget.

};

void mafTreeWidgetTest::mafTreeWidgetAllocationTest() {
    QVERIFY(m_TreeWidget != NULL);
    /// create hierarchy of vmes
    mafHierarchy *m_HierarchyToManage = mafNEW(mafCore::mafHierarchy);
/*int argc = 1;
QApplication app(argc, NULL);*/
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

    mafTreeModel model;
    model.setHierarchy(m_HierarchyToManage);
    m_TreeWidget->setModel( &model );
/*m_TreeWidget->show();
app.exec();*/


    mafDEL(m_HierarchyToManage);
    mafDEL(vmeChild1);
    mafDEL(vmeChild0Child0);
    mafDEL(vmeChild0);
    mafDEL(vmeRoot);
}

MAF_REGISTER_TEST(mafTreeWidgetTest);
#include "mafTreeWidgetTest.moc"
