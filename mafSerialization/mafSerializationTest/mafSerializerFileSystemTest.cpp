/*
 *  mafSerializerFileSystemTest.cpp
 *  mafSerializationTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafSerializerFileSystem.h>

using namespace mafCore;
using namespace mafSerialization;

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

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_Serializer = mafNEW(mafSerialization::mafSerializerFileSystem);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafFile::remove(m_TestURL);
        mafDEL(m_Serializer);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafSerializerFileSystem allocation test case.
    void mafSerializerFileSystemAllocationTest();
    /// Test the writing feature for the FileSystem Serializer
    void mafSerializerFileSystemWritingTest();
    /// Test the reading feature for the FileSystem Serializer
    void mafSerializerFileSystemReadingTest();

private:
    mafSerializerFileSystem *m_Serializer; ///< Test var
    mafString m_TestURL; ///< Test URL for file.
};

void mafSerializerFileSystemTest::mafSerializerFileSystemAllocationTest() {
    QVERIFY(m_Serializer != NULL);
}

void mafSerializerFileSystemTest::mafSerializerFileSystemWritingTest() {
    // Create the temporary file into the temp directory of the current user.
    mafString test_file;
    test_file = QDir::tempPath();
    test_file.append("/maf3Logs");
    QDir log_dir(test_file);
    if(!log_dir.exists()) {
        log_dir.mkpath(test_file);
    }
    test_file.append("/testFileSystemSerializer.maf3");

    // Create the DataStream used to stream data into the File.
    mafDataStream dataStream;

    // Assign the URL to the serializer.
    m_TestURL = "file:";
    m_TestURL.append(test_file);
    m_Serializer->setDataURL(m_TestURL);
    // Open the device to allow writing operations.
    m_Serializer->openDevice(mafSerializerOpenModeOut);
    // Connect the Serializer to the DataStream
    dataStream.setDevice(m_Serializer->ioDevice());

    // Send data into the stream.
    dataStream << mafString("Test1");
    dataStream << 2;

    // Close the connection to the File.
    m_Serializer->closeDevice();

    QVERIFY(mafFile::exists(test_file));
    QFileInfo fInfo(test_file);
    QVERIFY(fInfo.size() > 0);
}

void mafSerializerFileSystemTest::mafSerializerFileSystemReadingTest() {
    // Create the DataStream used to stream data into the File.
    mafDataStream dataStream;

    // Assign the URL to the serializer.
    m_Serializer->setDataURL(m_TestURL);
    // Open the device to allow reading operations.
    m_Serializer->openDevice(mafSerializerOpenModeIn);

    // Connect the Serializer to the DataStream
    dataStream.setDevice(m_Serializer->ioDevice());

    mafString strVar;
    int val = 0;

    dataStream >> strVar;
    dataStream >> val;

    // Close the connection to the File.
    m_Serializer->closeDevice();

    QVERIFY(val == 2);
    QVERIFY(strVar == "Test1");
}

MAF_REGISTER_TEST(mafSerializerFileSystemTest);
#include "mafSerializerFileSystemTest.moc"
