/*
 *  mafNetworkConnectorTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 09/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafNetworkConnector.h>

class testNetworkConnectorCustom;

/**
 Class name: mafNetworkConnectorTest
 This class implements the test suite for mafNetworkConnector.
 */

 //! <title>
//mafNetworkConnector
//! </title>
//! <description>
//mafNetworkConnector is the interface class for client/server objects that
//works over network.
//! </description>


class mafNetworkConnectorTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafNetworkConnector allocation test case.
    void mafNetworkConnectorAllocationTest();
    /// Test the creation of client and server.
    void mafNetworkConnectorCreateClientAndServerTest();
    /// test the function that retrive protocol type
    void retrieveProtocolTest();

private:
    mafEventBus::mafNetworkConnector *m_NetworkConnector; ///< Test var.
};
