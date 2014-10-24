/*
 *  mafMessageHandlerTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafMessageHandlerTest::initTestCase() {
    m_Handler = mafMessageHandler::instance();
    m_Handler->installMessageHandler();
}

void mafMessageHandlerTest::cleanupTestCase() {
    m_Handler->shutdown();
}

void mafMessageHandlerTest::mafMessageHandlerConstructorTest() {
    QVERIFY(m_Handler != NULL);
}

void mafMessageHandlerTest::setActiveloggerTest() {
    mafRegisterObject(mafLoggerConsole);
    mafLogger *logger = new mafLoggerConsole();
    m_Handler->setActiveLogger(logger);

    QString cn = logger->metaObject()->className();
    QString res = "mafCore::mafLoggerConsole";

    // Default logger is mafLoggerConsole.
    QCOMPARE(cn, res);
    mafDEL(logger);
    m_Handler->setDeafultLoggerAsActive();
}

#include "mafMessageHandlerTest.moc"


