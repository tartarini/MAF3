/*
 *  mafTreeItemTest.h
 *  mafTreeItem
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>

#include <mafGUIRegistration.h>
#include <mafTreeItem.h>

/**
 Class name: mafTreeItemTest
 This class implements the test suite for mafTreeItem.
 */
class mafTreeItemTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// allocation test case.
    void mafTreeItemAllocationTest();

    /// retrieve data test
    void dataTest();

private:
    mafGUI::mafTreeItem *m_TreeItem; ///< Reference to the tree item.
    mafCore::mafObjectBase *m_BaseObject;
};
