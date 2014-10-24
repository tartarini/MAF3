/*
 *  mafExternalDataCodecTest.cpp
 *  mafSerializationTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

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
    m_ByteArray = m_CodecString.toLatin1();
    return m_ByteArray.data();
}
//------------------------------------------------------------------------------------------

void mafExternalDataCodecTest::initTestCase() {
    m_ExternalDataCodec = mafNEW(testExternalDataCodecCustom);
}

/// Cleanup test variables memory allocation.
void mafExternalDataCodecTest::cleanupTestCase() {
    mafDEL(m_ExternalDataCodec);
}

void mafExternalDataCodecTest::mafCodecAllocationTest() {
    QVERIFY(m_ExternalDataCodec != NULL);
}

void mafExternalDataCodecTest::encodeTest() {
    char * outputString = m_ExternalDataCodec->encode();
    QByteArray ba = m_ExternalDataCodec->codecString().toLatin1();
    bool compare = strcmp(outputString, ba.data()) == 0;
    QVERIFY(compare);
}

void mafExternalDataCodecTest::decodeTest() {
    QString res = "Coded data";

    // Call the encode function by passing the string as argument.
    QByteArray ba = res.toLatin1();
    m_ExternalDataCodec->decode(ba.data());

    QString stringReturned(m_ExternalDataCodec->codecString());

    QCOMPARE(res, stringReturned);
}

#include "mafExternalDataCodecTest.moc"


