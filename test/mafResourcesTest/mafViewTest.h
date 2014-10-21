/*
 *  mafViewTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafResourcesSingletons.h>
#include <mafProxy.h>
#include <mafView.h>
#include <mafToolHandler.h>
#include <mafVME.h>
#include <mafSceneNode.h>
#include <mafVisitorFindSceneNodeByVMEHash.h>

/**
 Class name: mafViewTest
 This class implements the test suite for mafView.
 */

//! <title>
//mafView
//! </title>
//! <description>
//mafView defines the base class for the view in MAF3.
//! </description>

class mafViewTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafView allocation test case.
    void mafViewAllocationTest();

    /// mafView add & remove mafSceneNode test case.
    void mafViewAddRemoveSceneNodeTest();

    /// mafView show scene node test case.
    void mafViewShowSceneNodeTest();

    /// mafView show scene node test case.
    void mafViewPlugVisualPipeTest();

    /// mafView setRenderingWidget test case.
    void mafViewRenderingWidgetTest();

private:
    mafResources::mafView *m_View; ///< Test var.
    QVariantHash m_BindingHash; ///< Test var.
    mafEventBus::mafEventBusManager *m_EventBus;
    mafResources::mafVMEManager *m_VMEManager;
};
