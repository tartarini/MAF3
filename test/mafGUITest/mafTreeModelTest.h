/*
 *  mafTreeModelTest.h
 *  mafTreeModel
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>

#include <mafResourcesDefinitions.h>
#include <mafVME.h>

#include <mafGUIRegistration.h>
#include <mafTreeModel.h>


/**
 Class name: mafTreeModelTest
 This class implements the test suite for mafTreeModel.
 */
class mafTreeModelTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// allocation test case.
    void mafTreeModelAllocationTest();

    /// hierarchy set/get Test case and insert Item in model.
    void hierarchyAndInsertItemTest();

    /// clearing the model test
    void clearTest();

private:
    mafGUI::mafTreeModel *m_TreeModel; ///< Reference to the tree item.
};
