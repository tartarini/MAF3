/*
 *  mafCodecRawBinaryTest.h
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
#include <mafCodecRawBinary.h>
#include <mafObject.h>
#include <mafMementoObject.h>

/**
 Class name: mafCodecRawBinaryTest
 This class implements the test suite for mafCodecRawBinary.
 */

//! <title>
//mafCodecRawBinary
//! </title>
//! <description>
//mafCodecRawBinary is a codec to Encode/Decode maf objects into a raw memory dump.
//! </description>

class mafCodecRawBinaryTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafCodecRawBinary allocation test case.
    void mafCodecRawBinaryAllocationTest();

    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();


private:
    mafSerialization::mafCodecRawBinary *m_CodecRaw; ///< Test var
    mafCore::mafObject *m_ObjectTest; ///< Test Object.
    QBuffer m_Buffer; ///< Test buffer data.
    QVariantList m_List; ///< Test QList.
};

