/*
 *  mafSerializerFileSystemTest.cpp
 *  mafSerializationTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSerializationTestList.h"

using namespace mafCore;
using namespace mafSerialization;

void mafSerializerFileSystemTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_Serializer = mafNEW(mafSerialization::mafSerializerFileSystem);
}

void mafSerializerFileSystemTest::cleanupTestCase() {
    QFile::remove(m_TestURL);
    mafDEL(m_Serializer);
    mafMessageHandler::instance()->shutdown();
}

void mafSerializerFileSystemTest::mafSerializerFileSystemAllocationTest() {
    QVERIFY(m_Serializer != NULL);
}

void mafSerializerFileSystemTest::mafSerializerFileSystemWritingTest() {
    // Create the temporary file into the temp directory of the current user.
    QString test_file;
    test_file = QDir::tempPath();
    test_file.append("/maf3Logs");
    QDir log_dir(test_file);
    if(!log_dir.exists()) {
        log_dir.mkpath(test_file);
    }
    test_file.append("/testFileSystemSerializer.maf3");

    // Create the DataStream used to stream data into the File.
    QDataStream dataStream;

    // Assign the URL to the serializer.
    m_TestURL = "file:";
    m_TestURL.append(test_file);
    m_Serializer->setDataURL(m_TestURL);
    // Open the device to allow writing operations.
    m_Serializer->openDevice(mafSerializerOpenModeOut);
    // Connect the Serializer to the DataStream
    dataStream.setDevice(m_Serializer->ioDevice());

    // Send data into the stream.
    dataStream << QString("Test1");
    dataStream << 2;

    // Close the connection to the File.
    m_Serializer->closeDevice();

    QVERIFY(QFile::exists(test_file));
    QFileInfo fInfo(test_file);
    QVERIFY(fInfo.size() > 0);
}

void mafSerializerFileSystemTest::mafSerializerFileSystemReadingTest() {
    // Create the DataStream used to stream data into the File.
    QDataStream dataStream;

    // Assign the URL to the serializer.
    m_Serializer->setDataURL(m_TestURL);
    // Open the device to allow reading operations.
    m_Serializer->openDevice(mafSerializerOpenModeIn);

    // Connect the Serializer to the DataStream
    dataStream.setDevice(m_Serializer->ioDevice());

    QString strVar;
    int val = 0;

    dataStream >> strVar;
    dataStream >> val;

    // Close the connection to the File.
    m_Serializer->closeDevice();

    QVERIFY(val == 2);
    QVERIFY(strVar == "Test1");
}

#include "mafSerializerFileSystemTest.moc"
