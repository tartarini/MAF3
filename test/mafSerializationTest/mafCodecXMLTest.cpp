/*
 *  mafCodecXMLTest.cpp
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 21/01/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCodecXML.h>
#include <mafObject.h>
#include <mafMementoObject.h>

using namespace mafCore;
using namespace mafSerialization;

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
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_ObjectTest = mafNEW(mafCore::mafObject);
        m_CodecXML = mafNEW(mafSerialization::mafCodecXML);
        m_List.push_back("one");
        m_List.push_back("two");
        m_List.push_back("three");
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_ObjectTest);
        mafDEL(m_CodecXML);
        m_List.clear();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafCodecXML allocation test case.
    void mafCodecXMLAllocationTest();
    /// test the encode method.
    void encodeTest();
    /// test the decode method.
    void decodeTest();

private:
    mafCodecXML *m_CodecXML; ///< Test var
    mafObject *m_ObjectTest; ///< Test Object.
    QBuffer buffer; ///< Test buffer data.
    QVariantList m_List;
};

void mafCodecXMLTest::mafCodecXMLAllocationTest() {
    QVERIFY(m_CodecXML != NULL);
}

void mafCodecXMLTest::encodeTest() {
    // Create a test object to be serialized.
    m_ObjectTest->setObjectName("Test Object");
    m_ObjectTest->addTag("tag1");
    m_ObjectTest->addTag("tag2");
    m_ObjectTest->setTagList(m_List);

    m_ObjectTest->dictionary()->insert("nome", "geisi");
    m_ObjectTest->dictionary()->insert("cognome", "fico");

    // Ask for its memento (object state).
    mafMemento *m1 = m_ObjectTest->createMemento();
    mafMemento *m2 = m_ObjectTest->createMemento();
    m2->setParent(m1);

    // Open the buffer in writing modality
    buffer.open(QBuffer::WriteOnly);

    // Assign the IODevice and give the memento to the codec.
    //! <snippet>
    m_CodecXML->setDevice(&buffer);
    m_CodecXML->encode(m1);
    //! </snippet>
    // Close the buffer.
    buffer.close();


    int s = buffer.size();

    QVERIFY(s > 0);

    qDebug() << "\n" << buffer.data();

    mafDEL(m2);
    mafDEL(m1);
}

void mafCodecXMLTest::decodeTest() {
    mafRegisterObject(mafCore::mafMementoObject);

    // Reopen the buffer in reading mode.
    buffer.open(QBuffer::ReadOnly);
    // Check the size
    int s = buffer.size();
    QVERIFY(s > 0);

    // Assign the IODevice.
    m_CodecXML->setDevice(&buffer);

    // Decode the information
    mafMemento *m = m_CodecXML->decode();

    // Close the buffer.
    buffer.close();

    QVERIFY(m != NULL);
    int numOfChildren = m->children().count();
    QVERIFY(numOfChildren == 1);

    mafObject *obj = mafNEW(mafCore::mafObject);
    obj->setMemento(m, true);

    QVERIFY(m_ObjectTest->isEqual(obj));

    //Check decoding of the list
    int i = 0;
    for (; i < m_List.size(); ++i) {
        QVERIFY(m_List.at(i).toString().compare(obj->tagList()->at(i).toString()) == 0);

    }
    mafDEL(obj);
    mafDEL(m);
}

MAF_REGISTER_TEST(mafCodecXMLTest);
#include "mafCodecXMLTest.moc"

