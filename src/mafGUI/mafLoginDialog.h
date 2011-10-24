/*
 *  mafLoginDialog.h
 *  mafGUI
 *
 *  Created by Roberto Mucci on 26/10/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGINDIALOG_H
#define MAFLOGINDIALOG_H

#include "mafGUIDefinitions.h"
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox.h>
#include <QGridLayout>
#include <QStringList>
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
 
    /// Sets a list of allowed usernames from which the user can pick one if he does not want to directly edit it.
    void setUsernamesList( const QStringList& usernames );
 
    Q_SIGNALS:
     /// Signal emitted when the login is performed.
    void acceptLoginSignal( QString& username, QString& password, int& indexNumber );

    void abortLoginSignal();
 
protected Q_SLOTS:
    /// A lot to adjust the emitting of the signal.
    void slotAcceptLogin();

    /// A lot to adjust the emitting of the signal.
    void slotAbortLogin();

    /// Method to set up all dialog components and initialize them.
    void setUpGUI();

    private:
    QLabel* m_LabelUsername; ///< A label for the username component.
    QLabel* m_LabelPassword; ///< A label for the password.
    QComboBox* m_ComboUsername; ///< * Eeditable combo box.
    QLineEdit* m_EditPassword; ///< Field to let the user enters his password.
    QDialogButtonBox* m_Buttons; ///< The standard dialog button box.
};

} // end namespace

#endif // MAFLOGINDIALOG_H
