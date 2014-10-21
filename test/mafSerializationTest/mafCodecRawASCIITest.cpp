/*
 *  mafCodecRawASCIITest.cpp
 *  mafSerializationTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

 
#include "mafSerializationTestList.h"

using namespace mafCore;
using namespace mafSerialization;

void mafCodecRawASCIITest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_ObjectTest = mafNEW(mafCore::mafObject);
    m_CodecRaw = mafNEW(mafSerialization::mafCodecRawASCII);
    m_List.push_back("one");
    m_List.push_back("two");
    m_List.push_back("three");
}

void mafCodecRawASCIITest::cleanupTestCase() {
    mafDEL(m_ObjectTest);
    mafDEL(m_CodecRaw);
    m_List.clear();
    mafMessageHandler::instance()->shutdown();
}

void mafCodecRawASCIITest::mafCodecRawBinaryAllocationTest() {
    QVERIFY(m_CodecRaw != NULL);
}

void mafCodecRawASCIITest::encodeTest() {
    // Create a test object to be serialized.
    //! <snippet>
    m_ObjectTest->setObjectName("Test Object");
    m_ObjectTest->addTag("tag1");
    m_ObjectTest->addTag("tag2");
    m_ObjectTest->setTagList(m_List);
    //! </snippet>

    m_ObjectTest->dictionary()->insert("nome", "geisi");
    m_ObjectTest->dictionary()->insert("cognome", "fico");

    // Ask for its memento (object state).
    mafMemento *m1 = m_ObjectTest->createMemento();
    mafMemento *m2 = m_ObjectTest->createMemento();

    m2->setParent(m1);

    //! <snippet>
    //// Open the buffer in writing modality
    m_Buffer.open(QBuffer::WriteOnly);

    //// Assign the IODevice.
    m_CodecRaw->setDevice(&m_Buffer);

    //// Give the memento to the codec.
    m_CodecRaw->encode(m1);
    //// Close the buffer.
    m_Buffer.close();
    //! </snippet>

    int s = m_Buffer.size();

    QVERIFY(s > 0);

    mafDEL(m2);
    mafDEL(m1);
}

void mafCodecRawASCIITest::decodeTest() {
    mafRegisterObject(mafCore::mafMementoObject);

    // Reopen the buffer in reading mode.
    m_Buffer.open(QBuffer::ReadOnly);
    // Check the size
    int s = m_Buffer.size();
    QVERIFY(s > 0);

    // Assign the IODevice.
    m_CodecRaw->setDevice(&m_Buffer);

    // Decode the information
    mafMemento *m = m_CodecRaw->decode();

    // Close the buffer.
    m_Buffer.close();

    QVERIFY(m != NULL);
    int numOfChildren = m->children().count();
    QVERIFY(numOfChildren == 1);

    mafObject *obj = mafNEW(mafCore::mafObject);
    obj->setMemento(m, true);

    QVERIFY(m_ObjectTest->isEqual(obj));

    //Check decoding of the list
    int i = 0;
    for (; i < m_List.size(); ++i) {
//        QString s = m_List.at(i).toString();
//        QString t = obj->tagList()->at(i).toString();
        QVERIFY(m_List.at(i).toString().compare(obj->tagList()->at(i).toString()) == 0);
    }

	mafDEL(m); // LEAK OR CRASH IN VS2013
	mafDEL(obj);
}

#include "mafCodecRawASCIITest.moc"

