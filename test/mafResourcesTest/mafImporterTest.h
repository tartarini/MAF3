/*
 *  mafImporterTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafImporter.h>

/**
 Class name: mafImporterTest
 This class implements the test suite for mafImporter.
 */

//! <title>
// mafImporterTest
//! </title>
//! <description>
// mafImporterTest represents the test suite for mafImporter.
//! </description>
class mafImporterTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafImporter allocation test case.
    void mafImporterAllocationTest();
    
    /// acceptObject test case.
    void acceptObjectTest();
    
    /// filename test.
    void filenameTest();

private:
    mafResources::mafImporter *m_Importer; ///< Test var.
};
