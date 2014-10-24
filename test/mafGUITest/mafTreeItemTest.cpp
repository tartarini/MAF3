/*
 *  mafTreeItemTest.cpp
 *  mafTreeItem
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUITestList.h"

using namespace mafCore;
using namespace mafGUI;

void mafTreeItemTest::initTestCase() {
    // Register all the creatable objects for the mafGUI module.
    mafGUIRegistration::registerGUIObjects();
    m_BaseObject = mafNEW(mafCore::mafObjectBase);
    m_BaseObject->setObjectName("TestObjectName");
    m_TreeItem = new mafTreeItem(m_BaseObject);
}

void mafTreeItemTest::cleanupTestCase() {
    delete m_TreeItem;
    mafDEL(m_BaseObject);
}

void mafTreeItemTest::mafTreeItemAllocationTest() {
    QVERIFY(m_TreeItem != NULL);
}

void mafTreeItemTest::dataTest() {
    QVERIFY(m_TreeItem->data() != NULL);
    QVERIFY(m_TreeItem->data()->objectName() == "TestObjectName");
}

#include "mafTreeItemTest.moc"

