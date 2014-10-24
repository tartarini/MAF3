/*
 *  mafCodecXMLTest.h
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
#include <mafCodecXML.h>
#include <mafObject.h>
#include <mafMementoObject.h>

/**
 Class name: mafCodecXMLTest
 This class implements the test suite for mafCodecXML.
 */

//! <title>
//mafCodecXML
//! </title>
//! <description>
//mafCodecXML is a codec to Encode/Decode maf objects into XML structure.
//! </description>

class mafCodecXMLTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafCodecXML allocation test case.
    void mafCodecXMLAllocationTest();
    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();

private:
    mafSerialization::mafCodecXML *m_CodecXML; ///< Test var
    mafCore::mafObject *m_ObjectTest; ///< Test Object.
    QBuffer buffer; ///< Test buffer data.
    QVariantList m_List;
};
