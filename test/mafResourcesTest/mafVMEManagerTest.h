/*
 *  mafViewManagerTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafEventBusManager.h>
#include <mafVMEManager.h>
#include <mafMatrix4x4.h>
#include <mafVME.h>
#include <mafDataSet.h>

/**
 Class name: mafVMEManagerTest
 This class implements the test suite for mafVMEManager.
 */
class mafVMEManagerTest : public QObject {
    Q_OBJECT

/// create a huge vme tree for benchmark.
void unbalancedTreeRandomCreation(unsigned int numberOfElements); 
    
private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();
    
    /// mafVMEManager allocation test case.
    void mafVMEManagerAllocationTest();
    /// VME managing test.
    void vmeManagingTest();
    /// create hierarchy Test
    void createHierarchyTest();
    /// absolute pose matrix Test
    void absolutePoseMatrixTest();
    /// benchmarking the absolute matrix
    void benchmarkarkedAbsoluteMatrixTest();

private:
    mafResources::mafVMEManager *m_VMEManager;
    mafEventBus::mafEventBusManager *m_EventBus;
    mafCore::mafHierarchy *m_Hierarchy; ///< Test var.
    mafResources::mafVME *m_SelectedVME;
};
