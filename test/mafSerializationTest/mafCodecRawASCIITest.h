/*
 *  mafCodecRawASCIITest.h
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
#include <mafCodecRawASCII.h>
#include <mafObject.h>
#include <mafMementoObject.h>

/**
 Class name: mafCodecRawASCIITest
 This class implements the test suite for mafCodecRawASCII.
 */

//! <title>
//mafCodecRawASCII
//! </title>
//! <description>
//mafCodecRawASCII is a codec to Encode/Decode maf objects into a raw memory dump.
//! </description>

class mafCodecRawASCIITest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafCodecRawASCII allocation test case.
    void mafCodecRawBinaryAllocationTest();
    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();


private:
    mafSerialization::mafCodecRawASCII *m_CodecRaw; ///< Test var
    mafCore::mafObject *m_ObjectTest; ///< Test Object.
    QBuffer m_Buffer; ///< Test buffer data.
    QVariantList m_List; ///< Test QList.
};
