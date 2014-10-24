/*
 *  mafObjectBaseTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObjectBase.h>

using namespace mafCore;

//forward declaration
class testObjectBaseCustom;

/**
 Class name: mafObjectBaseTest
 This class implements the test suite for mafObjectBase.
 */

//! <title>
//mafObjectBase
//! </title>
//! <description>
//mafObjectBase defines the MAF3 base object.
//Its main function is to provide a unique ID (mafId) to each mafObject and to add the object to the mafObjectRegistry.
//! </description>

class mafObjectBaseTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Test the ID generation for the mafObjectBase
    void objectIdTest();
    /// Test the creation of the objectHash.
//    void createObjectHashTest();
    /// Test the isEqual method.
    void isEqualTest();

    /// Modified test
    void modifiedTest();

private:
    mafObjectBase *m_ObjTestVar; ///< Test variable
};
