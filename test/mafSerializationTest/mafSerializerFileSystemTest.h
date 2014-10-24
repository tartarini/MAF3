/*
 *  mafSerializerFileSystemTest.cpp
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafSerializerFileSystem.h>

/**
 Class name: mafSerializerFileSystemTest
 This class implements the test suite for mafSerializerFileSystem.
 */

//! <title>
//mafSerializerFileSystem
//! </title>
//! <description>
//This class is responsible to write input DataStream into a file based on local filesystem.
//! </description>

class mafSerializerFileSystemTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafSerializerFileSystem allocation test case.
    void mafSerializerFileSystemAllocationTest();
    /// Test the writing feature for the FileSystem Serializer
    void mafSerializerFileSystemWritingTest();
    /// Test the reading feature for the FileSystem Serializer
    void mafSerializerFileSystemReadingTest();

private:
    mafSerialization::mafSerializerFileSystem *m_Serializer; ///< Test var
    QString m_TestURL; ///< Test URL for file.
};
