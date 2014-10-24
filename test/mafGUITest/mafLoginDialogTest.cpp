/*
 *  mafLoginDialogTest.cpp
 *  mafLoginDialog
 *
 *  Created by Roberto Mucci - Daniele Giunchi on 08/12/11.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUITestList.h"

using namespace mafCore;
using namespace mafGUI;

void mafLoginDialogTest::initTestCase() {
    // Register all the creatable objects for the mafGUI module.
    mafGUIRegistration::registerGUIObjects();
    m_LoginDialog = new mafLoginDialog();
    bool ret = connect( m_LoginDialog,SIGNAL(acceptLoginSignal(QString&,QString&, bool)), this,SLOT(slotAcceptUserLogin(QString&,QString&, bool)));
}

void mafLoginDialogTest::cleanupTestCase() {
    delete m_LoginDialog;
}

void mafLoginDialogTest::mafLoginDialogAllocationTest() {
    QVERIFY(m_LoginDialog != NULL);
}

void mafLoginDialogTest::credentialTest() {
    QString user = "testUser";
    QString password = "testPassword";

    m_LoginDialog->setUsername(user);
    m_LoginDialog->setPassword(password);
    m_LoginDialog->setRemember(true);
    m_LoginDialog->slotAcceptLogin();
}

void mafLoginDialogTest::slotAcceptUserLogin( QString& username, QString& password, bool remember) {

    QVERIFY(username.compare("testUser") == 0);
    QVERIFY(password.compare("testPassword") == 0);
    QVERIFY(remember == true);
}

#include "mafLoginDialogTest.moc"

