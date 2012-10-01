/*
 *  mafExternalDataCodecTest.cpp
 *  mafSerializationTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafExternalDataCodec.h>

using namespace mafCore;

//------------------------------------------------------------------------------------------
/**
 Class name: testExternalDataCodecCustom
 This class implements the external data codec to be tested.
 */
class  testExternalDataCodecCustom : public  mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    testExternalDataCodecCustom(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode the output type into the memento.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

    /// Return the string variable encoded and decoded from the mafCodecCustom.
    QString codecString() {return m_CodecString;}

private:
    QString m_CodecString; ///< Test Var.
    QByteArray m_ByteArray; ///< Test var.
};

testExternalDataCodecCustom::testExternalDataCodecCustom(const QString code_location) : mafExternalDataCodec(code_location) {
}

void testExternalDataCodecCustom::decode(const char *input_string, bool binary) {
    Q_UNUSED(binary);
    REQUIRE(input_string != NULL);
    m_CodecString.clear();
    m_CodecString.append(input_string);
}

char * testExternalDataCodecCustom::encode(bool binary) {
    Q_UNUSED(binary);
    m_CodecString.clear();
    m_CodecString.append("Coded data");
    m_ByteArray = m_CodecString.toAscii();
    return m_ByteArray.data();
}
//------------------------------------------------------------------------------------------

/**
 Class name: mafExternalDataCodecTest
 This class implements the test suite for mafExternalDataCodec.
 */
class mafExternalDataCodecTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_ExternalDataCodec = mafNEW(testExternalDataCodecCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_ExternalDataCodec);
    }

    /// mafCodec allocation test case.
    void mafCodecAllocationTest();
    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();

private:
    testExternalDataCodecCustom *m_ExternalDataCodec; ///< Test var
    QDataStream m_StreamExternal; ///< Test buffer data.
    QDataStream m_StreamInExternal; ///< Test buffer data.
};

void mafExternalDataCodecTest::mafCodecAllocationTest() {
    QVERIFY(m_ExternalDataCodec != NULL);
}

void mafExternalDataCodecTest::encodeTest() {
    char * outputString = m_ExternalDataCodec->encode();
    QByteArray ba = m_ExternalDataCodec->codecString().toAscii();
    bool compare = strcmp(outputString, ba.data()) == 0;
    QVERIFY(compare);
}

void mafExternalDataCodecTest::decodeTest() {
    QString res = "Coded data";

    // Call the encode function by passing the string as argument.
    QByteArray ba = res.toAscii();
    m_ExternalDataCodec->decode(ba.data());

    QString stringReturned(m_ExternalDataCodec->codecString());

    QCOMPARE(res, stringReturned);
}

MAF_REGISTER_TEST(mafExternalDataCodecTest);
#include "mafExternalDataCodecTest.moc"


