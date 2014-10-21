/*
 *  mafLoginDialogTest.h
 *  mafLoginDialog
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>

#include <mafGUIRegistration.h>
#include <mafLoginDialog.h>

/**
 Class name: mafLoginDialogTest
 This class implements the test suite for mafLoginDialog.
 */
class mafLoginDialogTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();
    
    /// allocation test case.
    void mafLoginDialogAllocationTest();

    /// credential test
    void credentialTest();

public Q_SLOTS:
    /// Intercept the login dialog signal.
    void slotAcceptUserLogin(QString& username, QString& password, bool remember);

private:
    mafGUI::mafLoginDialog *m_LoginDialog; ///< login dialog.
    
};
