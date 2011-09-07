
/*
 *  mafCSVReaderTest.cpp
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 25/01/2011.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCSVReader.h>

using namespace mafCore;
using namespace mafSerialization;

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
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_CSVReader = mafNEW(mafSerialization::mafCSVReader);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CSVReader);
    }

    /// mafCodecRawASCII allocation test case.
    void mafCSVReaderAllocationTest();

    /// test the decode method.
    void csvFromFileTest();

    /// test the encode method.
    void csvFromStringTest();


private:
    mafCSVReader *m_CSVReader; ///< Test var
};

void mafCSVReaderTest::mafCSVReaderAllocationTest() {
    QVERIFY(m_CSVReader != NULL);
}

void mafCSVReaderTest::csvFromFileTest() {
    QFile file("import.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<< "cannot read file!";
    }
    m_CSVReader->setDevice(&file);

    int count = 0;
    while(1) {
        const QStringList &sl = m_CSVReader->parseLine();
        if(sl.count() == 0) {
            break;
        }
        qDebug() << sl;
        count++;
    }

    qDebug() << count;
    QVERIFY(count == 45); //44 lines for data + 1 line for header

}

void mafCSVReaderTest::csvFromStringTest() {
    QString csvString = "Programmer,Society,Framework\n";
    csvString.append("Daniele Giunchi,SCS,MAF3\n");
    m_CSVReader->setString(csvString);
    int count = 0;
    while(1) {
        const QStringList &sl = m_CSVReader->parseLine();
        if(sl.count() == 0) {
            break;
        }
        qDebug() << sl;
        count++;
    }

    QVERIFY(count == 2); //two lines
}


//MAF_REGISTER_TEST(mafCSVReaderTest);
#include "mafCSVReaderTest.moc"

