/*
 *  mafLoginDialog.h
 *  mafGUI
 *
 *  Created by Roberto Mucci on 26/10/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGINDIALOG_H
#define MAFLOGINDIALOG_H

#include "mafGUIDefinitions.h"
#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QProgressDialog>
#include <QGridLayout>
#include <QDebug>

namespace mafGUI {

//forward class
class mafSyntaxHighlighter;

/**
 Class name: mafLoginDialog
 This class defines login dialog to authenticate a user inserting
 username and password.
 */
class MAFGUISHARED_EXPORT mafLoginDialog : public QDialog {
    Q_OBJECT
    /// typedef macro.

public:
    /// Object constructor.
     mafLoginDialog(QWidget *parent = 0);
 
    /// Sets the username.
    void setUsername( QString& username );
 
    /// Sets the password.
    void setPassword( QString& password );

    ///Sets the checkbox status.
    void setRemember(bool rememberFalg);

    ///Show progress bar.
    void showProgressBar();

    ///Close progress bar.
    void closeProgressBar();
 
    Q_SIGNALS:
     /// Signal emitted when the login is performed.
    void acceptLoginSignal( QString& username, QString& password, bool rememebr);

    void abortLoginSignal();
 
public Q_SLOTS:
    /// A lot to adjust the emitting of the signal.
    void slotAcceptLogin();

    /// A lot to adjust the emitting of the signal.
    void slotAbortLogin();

    /// Method to set up all dialog components and initialize them.
    void setUpGUI();

private:
    /// Allows to write the updated preferences.
    void writeSettings(void);

    /// Allows to read the saved preferences.
    void readSettings(void);

    QLineEdit *m_EditUsername; ///< Field to let the user enters his userName.
    QLineEdit *m_EditPassword; ///< Field to let the user enters his password.
    QCheckBox *m_Checkbox; ///< Checkbox to remember userName and password.
    QDialogButtonBox *m_Buttons; ///< The standard dialog button box.
    QProgressDialog *m_Progress; ///< Progress bar.
};

} // end namespace

#endif // MAFLOGINDIALOG_H
