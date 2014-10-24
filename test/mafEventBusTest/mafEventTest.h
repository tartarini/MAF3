
/*
 *  mafEventTest.h
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009-2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafEvent.h>

/**
 Class name: mafEventTest
 This class implements the test suite for mafEvent.
 */

//! <title>
//mafEvent
//! </title>
//! <description>
//mafEvent is the object which contain information in a dictionary structure,
//regarding message between classes through mafEventBus
//! </description>

class mafEventTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafEventDispatcherRemote allocation test case.
    void mafEventAllocationTest();

    /// test all the accessors of a mafEvent
    void mafEventAccessorsTest();
    
    /// Test arguments and retval methods
    void mafEventArgListRetValTest();

private:
    mafEventBus::mafEvent *m_Event; ///< Test var.
};
