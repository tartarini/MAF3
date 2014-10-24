/*
 *  mafCodecTest.cpp
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

//------------------------------------------------------------------------------------------
/**
 Class name: testCodecCustom
 This class implements the codec to be tested.
 */
class  testCodecCustom : public  mafCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafSerialization::mafCodec);

public:
    /// Object constructor.
    testCodecCustom(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafMemento *decode();

    /// Return the string variable encoded and decoded from the testCodecCustom.
    QString codecString() {return m_CodecString;}

private:
    QString m_CodecString; ///< Test Var.
};

testCodecCustom::testCodecCustom(const QString code_location) : mafCodec(code_location), m_CodecString("") {
    m_EncodingType = "CUSTOM";
}

mafMemento *testCodecCustom::decode() {
    m_CodecString = "Decoded";
    return NULL;
}

void testCodecCustom::encode(mafMemento *memento) {
    Q_UNUSED(memento);
    m_CodecString = "Encoded";
}
//------------------------------------------------------------------------------------------


void mafCodecTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_Codec = mafNEW(testCodecCustom);
}

void mafCodecTest::cleanupTestCase() {
    mafDEL(m_Codec);
    mafMessageHandler::instance()->shutdown();
}

void mafCodecTest::mafCodecAllocationTest() {
    QVERIFY(m_Codec != NULL);
    QString res("CUSTOM");
    QCOMPARE(m_Codec->encodingType(), res);
}

void mafCodecTest::encodeTest() {
    QString res("Encoded");
    m_Codec->encode(NULL);
    QCOMPARE(m_Codec->codecString(), res);
}

void mafCodecTest::decodeTest() {
    QString res("Decoded");
    m_Codec->decode();
    QCOMPARE(m_Codec->codecString(), res);
}

#include "mafCodecTest.moc"

