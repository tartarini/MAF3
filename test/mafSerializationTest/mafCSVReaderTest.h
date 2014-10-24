/*
 *  mafCSVReaderTest.h
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 10/07/2014.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCSVReader.h>

/**
 Class name: mafCSVReaderTest
 This class implements the test suite for mafCodecRawASCII.
 */

//! <title>
//mafCSVReader
//! </title>
//! <description>
//mafCSVReader is help class in order to read Comma Separated Value file. Currently it is only a tool that will be used from a mafCodec
//! </description>

class mafCSVReaderTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafCodecRawASCII allocation test case.
    void mafCSVReaderAllocationTest();

    /// test the decode method.
    void csvFromFileTest();

    /// test the encode method.
    void csvFromStringTest();


private:
    mafSerialization::mafCSVReader *m_CSVReader; ///< Test var
};
