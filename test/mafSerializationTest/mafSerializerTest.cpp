/*
 *  mafSerializerTest.cpp
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
#include <mafSerializer.h>

using namespace mafCore;
using namespace mafSerialization;

//------------------------------------------------------------------------------------------
/**
 Class name: testSerializerCustom
 This class implements the codec to be tested.
 */
class  testSerializerCustom : public  mafSerializer {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafSerialization::mafSerializer);

public:
    /// Object constructor.
    testSerializerCustom(const QString code_location = "");

    /// Initialize the IO device and make it ready to operate.
    /*virtual*/ void openDevice(mafSerializerOpenMode mode);

    /// Close the IO device and flush all the information into the media.
    /*virtual*/ void closeDevice();

    /// Return the string variable initializated on openDevice call of the testSerializerCustom.
    QString serializerString() {return m_SerializerString;}

private:
    QString m_SerializerString; ///< Test Var.
};

testSerializerCustom::testSerializerCustom(const QString code_location) : mafSerializer(code_location), m_SerializerString("") {
}

void testSerializerCustom::openDevice(mafSerializerOpenMode mode) {
    Q_UNUSED(mode);
    m_SerializerString = "DeviceOpened";
}

void testSerializerCustom::closeDevice() {
    m_SerializerString = "DeviceClosed";
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafSerializerTest
 This class implements the test suite for mafSerializer.
 */

 //! <title>
//mafSerializer
//! </title>
//! <description>
//mafSerializer is the base class that define the interface for MAF3 serializer.
//! </description>

class mafSerializerTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_Serializer = mafNEW(testSerializerCustom);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Serializer);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafSerializer allocation test case.
    void mafSerializerAllocationTest();
    /// test the openDevice/closeDevice method.
    void openCloseDeviceTest();
    /// test the data URL assignment.
    void dataURLTest();

private:
    testSerializerCustom *m_Serializer; ///< Test var
};

void mafSerializerTest::mafSerializerAllocationTest() {
    QVERIFY(m_Serializer != NULL);
}

void mafSerializerTest::openCloseDeviceTest() {
    QString res("DeviceOpened");

    // Open the device
    m_Serializer->openDevice(mafSerializerOpenModeBidirectional);
    QCOMPARE(m_Serializer->serializerString(), res);

    // check for the device.
    QIODevice *dev = m_Serializer->ioDevice();
    QVERIFY(dev == NULL);

    // Close the device.
    m_Serializer->closeDevice();
    res = "DeviceClosed";
    QCOMPARE(m_Serializer->serializerString(), res);
}

void mafSerializerTest::dataURLTest() {
    QString urlString("http://www.mysite.com/");
    m_Serializer->setDataURL(urlString);
    QUrl url = m_Serializer->dataURL();
    QVERIFY(!url.isEmpty());
    QString res = url.toString();
    QCOMPARE(res, urlString);
}

MAF_REGISTER_TEST(mafSerializerTest);
#include "mafSerializerTest.moc"

