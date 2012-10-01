/*
 *  mafLoginDialogTest.cpp
 *  mafLoginDialog
 *
 *  Created by Roberto Mucci on 08/12/11.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>

#include <mafGUIRegistration.h>
#include <QtGui>
#include <mafLoginDialog.h>

using namespace mafCore;
using namespace mafGUI;

/**
 Class name: mafLoginDialogTest
 This class implements the test suite for mafLoginDialog.
 */
class mafLoginDialogTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        // Register all the creatable objects for the mafGUI module.
        mafGUIRegistration::registerGUIObjects();
        m_LoginDialog = new mafLoginDialog();
        bool ret = connect( m_LoginDialog,SIGNAL(acceptLoginSignal(QString&,QString&, bool)), this,SLOT(slotAcceptUserLogin(QString&,QString&, bool)));
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_LoginDialog;
    }

    /// allocation test case.
    void mafLoginDialogAllocationTest();

    /// credential test
    void credentialTest();

public Q_SLOTS:
    /// Intercept the login dialog signal.
    void slotAcceptUserLogin(QString& username, QString& password, bool remember);

private:
    mafLoginDialog *m_LoginDialog; ///< login dialog.
    
};

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

MAF_REGISTER_TEST(mafLoginDialogTest);
#include "mafLoginDialogTest.moc"

