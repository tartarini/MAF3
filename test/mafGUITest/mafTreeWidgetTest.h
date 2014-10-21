/*
 *  mafTreeWidgetTest.h
 *  mafTreeWidget
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2010 SCS-B3C. All rights reserved.
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
#include <mafTreeWidget.h>
#include <mafTreeModel.h>

/**
 Class name: mafTreeWidgetTest
 This class implements the test suite for mafTreeWidget.
 */
class mafTreeWidgetTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// allocation test case.
    void mafTreeWidgetAllocationTest();

private:
    mafGUI::mafTreeWidget *m_TreeWidget; ///< Reference to the Tree Widget.

};
