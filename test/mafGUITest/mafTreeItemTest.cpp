/*
 *  mafTreeItemTest.cpp
 *  mafTreeItem
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>

#include <mafGUIRegistration.h>
#include <QtGui>
#include <mafTreeItem.h>

using namespace mafCore;
using namespace mafGUI;

/**
 Class name: mafTreeItemTest
 This class implements the test suite for mafTreeItem.
 */
class mafTreeItemTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Register all the creatable objects for the mafGUI module.
        mafGUIRegistration::registerGUIObjects();
        m_BaseObject = mafNEW(mafCore::mafObjectBase);
        m_BaseObject->setObjectName("TestObjectName");
        m_TreeItem = new mafTreeItem(m_BaseObject, false);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_TreeItem;
        mafDEL(m_BaseObject);
    }

    /// allocation test case.
    void mafTreeItemAllocationTest();

    /// retrieve data test
    void dataTest();

private:
    mafTreeItem *m_TreeItem; ///< Reference to the tree item.
    mafObjectBase *m_BaseObject;
};

void mafTreeItemTest::mafTreeItemAllocationTest() {
    QVERIFY(m_TreeItem != NULL);
}

void mafTreeItemTest::dataTest() {
    QVERIFY(m_TreeItem->data() != NULL);
    QVERIFY(m_TreeItem->data()->objectName() == "TestObjectName");
}

MAF_REGISTER_TEST(mafTreeItemTest);
#include "mafTreeItemTest.moc"

