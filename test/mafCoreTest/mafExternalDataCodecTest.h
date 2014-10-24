/*
 *  mafExternalDataCodecTest.h
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafExternalDataCodec.h>

//forward declaration
class testExternalDataCodecCustom;

/**
 Class name: mafExternalDataCodecTest
 This class implements the test suite for mafExternalDataCodec.
 */
class mafExternalDataCodecTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

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
