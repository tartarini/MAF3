/*
 *  mafVMETest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafResourcesSingletons.h>
#include <mafPipeData.h>
#include <mafProxy.h>
#include <mafInteractor.h>
#include <mafMemento.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafVMEManager.h>

//forward declarations
class testVMEPipeDataCustom;
class testVMEConcurrentAccess;

/**
 Class name: mafVMETest
 This class implements the test suite for mafVME.
 */

 //! <title>
//mafVME
//! </title>
//! <description>
//mafVME defines the base class for the VMEs in MAF3.
//! </description>

class mafVMETest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafVME allocation test case.
    void mafVMEAllocationTest();

    /// Data pipe test suite
    void mafVMEDataPipeTest();

    /// Memento test suite.
    void mafVMEMementoTest();

    /// Interactor assignment test suite.
    void mafVMEInteractorTest();

    /// Output data test suite.
    void mafVMEOutputDataTest();
    
    /// Test the concurrent access to the VME.
    void mafVMEConcurrentAccessTest();

private:
    mafResources::mafVME *m_VME; ///< Test var.
    mafEventBus::mafEventBusManager *m_EventBus;
    mafResources::mafVMEManager *m_VMEManager;
};
