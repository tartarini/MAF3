
/*
 *  mafCSVReaderTest.cpp
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 25/01/2011.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafSerializationTestList.h"
#include "mafCSVReaderTest.h"

using namespace mafCore;
using namespace mafSerialization;

void mafCSVReaderTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_CSVReader = mafNEW(mafSerialization::mafCSVReader);
}

void mafCSVReaderTest::cleanupTestCase() {
    mafDEL(m_CSVReader);
    mafMessageHandler::instance()->shutdown();
}

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



#include "mafCSVReaderTest.moc"

