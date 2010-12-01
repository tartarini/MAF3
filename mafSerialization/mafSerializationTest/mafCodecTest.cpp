/*
 *  mafCodecTest.cpp
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
#include <mafCodec.h>

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
    testCodecCustom(const mafString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafMemento *decode();

    /// Return the string variable encoded and decoded from the testCodecCustom.
    mafString codecString() {return m_CodecString;}

private:
    mafString m_CodecString; ///< Test Var.
};

testCodecCustom::testCodecCustom(const mafString code_location) : mafCodec(code_location), m_CodecString("") {
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


/**
 Class name: mafCodecTest
 This class implements the test suite for mafCodec.
 */

 //! <title>
//mafCodec
//! </title>
//! <description>
//mafCodec is the base class that define the interface for MAF3 codec.
//! </description>

class mafCodecTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_Codec = mafNEW(testCodecCustom);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Codec);
    }

    /// mafCodec allocation test case.
    void mafCodecAllocationTest();
    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();

private:
    testCodecCustom *m_Codec; ///< Test var
};

void mafCodecTest::mafCodecAllocationTest() {
    QVERIFY(m_Codec != NULL);
    mafString res("CUSTOM");
    QCOMPARE(m_Codec->encodingType(), res);
}

void mafCodecTest::encodeTest() {
    mafString res("Encoded");
    m_Codec->encode(NULL);
    QCOMPARE(m_Codec->codecString(), res);
}

void mafCodecTest::decodeTest() {
    mafString res("Decoded");
    m_Codec->decode();
    QCOMPARE(m_Codec->codecString(), res);
}


MAF_REGISTER_TEST(mafCodecTest);
#include "mafCodecTest.moc"

