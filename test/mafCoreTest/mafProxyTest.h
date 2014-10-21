/*
 *  mafProxyTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafProxy.h>

class testCustomDataType;

/**
 Class name: mafProxyTest
 This class implements the test suite for mafProxy.
 */

//! <title>
//mafProxy
//! </title>
//! <description>
//mafProxy This class defines the base concrete class container for the MAF3 dataset values.
//! </description>

class mafProxyTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafProxyAllocationTest();
    /// Check internal data type existance.
    void mafProxyInternalDataTest();
    /// Check mafProxyInterface 'wrapped' into QVariant.
    void mafProxyToVariant();

private:
    //! <snippet>
    mafProxy<testCustomDataType> m_Wrapper; ///< Test var.
    //! </snippet>
    testCustomDataType *t;  ///< First object of external to MAF type.
    testCustomDataType *tw; ///< Second object of external to MAF type.
};
