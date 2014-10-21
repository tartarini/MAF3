/*
 *  mafCodecTest.h
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
#include <mafCodec.h>


//forward declaration
class  testCodecCustom;

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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafCodec allocation test case.
    void mafCodecAllocationTest();
    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();

private:
    testCodecCustom *m_Codec; ///< Test var
};
