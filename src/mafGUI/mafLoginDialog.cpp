/*
*  mafLoginDialog.cpp
*  mafGUI
*
*  Created by Roberto Mucci on 26/10/10.
*  Copyright 2010 B3C. All rights reserved.
*
*  See Licence at: http://tiny.cc/QXJ4D
*
*/


#include "mafLoginDialog.h"
#include <QLabel>

using namespace mafCore;
using namespace mafGUI;

mafLoginDialog::mafLoginDialog(QWidget *parent) :
QDialog(parent)
{
    setUpGUI();
    setWindowTitle( tr("User Login") );
    setModal( true );
}

void mafLoginDialog::setUpGUI(){
    // set up the layout
    QGridLayout* formGridLayout = new QGridLayout( this );

    // initialize the username combo box so that it is editable
    m_EditUsername = new QLineEdit( this );
    // initialize the password field so that it does not echo
    // characters
    m_EditPassword = new QLineEdit( this );
    m_EditPassword->setEchoMode( QLineEdit::Password );

    // initialize the labels
    QLabel *labelUsername = new QLabel( this );
    QLabel *m_LabelPassword = new QLabel( this );
    labelUsername->setText( tr( "Username" ) );
    labelUsername->setBuddy( m_EditUsername );
    m_LabelPassword->setText( tr( "Password" ) );
    m_LabelPassword->setBuddy( m_EditPassword );

    QLabel *rememberLabel = new QLabel( this );
    rememberLabel->setText(tr("remember me"));
    m_Checkbox = new QCheckBox();

    // initialize m_Buttons
    m_Buttons = new QDialogButtonBox( this );
    m_Buttons->addButton( QDialogButtonBox::Ok );
    m_Buttons->addButton( QDialogButtonBox::Cancel );
    m_Buttons->button( QDialogButtonBox::Ok )->setText( tr("Login") );
    m_Buttons->button( QDialogButtonBox::Cancel )->setText( tr("Abort") );

    // connects slots
    connect(m_Buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(slotAbortLogin()));
    connect(m_Buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(slotAcceptLogin()));

    // place components into the dialog
    formGridLayout->addWidget( labelUsername, 0, 0 );
    formGridLayout->addWidget( m_EditUsername, 0, 1 );
    formGridLayout->addWidget( m_LabelPassword, 1, 0 );
    formGridLayout->addWidget( m_EditPassword, 1, 1 );
    formGridLayout->addWidget( rememberLabel, 2, 0 );
    formGridLayout->addWidget( m_Checkbox, 2, 1 );
    formGridLayout->addWidget( m_Buttons, 3, 0, 1, 2 );

    setLayout( formGridLayout );
}

void mafLoginDialog::setUsername(QString &username){
    m_EditUsername->setText( username );
}

void mafLoginDialog::setPassword(QString &password){
    m_EditPassword->setText( password );
}

void mafLoginDialog::setRemember(bool rememberFalg){
    m_Checkbox->setChecked(rememberFalg);
}

void mafLoginDialog::slotAcceptLogin(){
    QString username = m_EditUsername->text();
    QString password = m_EditPassword->text();
    bool remember = m_Checkbox->isChecked();

    QProgressDialog progress("Copying files...", "Abort Copy", 0, numFiles, this);
    progress.setWindowModality(Qt::WindowModal);

    for (int i = 0; i < numFiles; i++) {
        progress.setValue(i);

        if (progress.wasCanceled())
            break;
        //... copy one file
    }
    progress.setValue(numFiles);

    Q_EMIT acceptLoginSignal( username, password, remember );
}

void mafGUI::mafLoginDialog::slotAbortLogin(){
    Q_EMIT abortLoginSignal();
    // close this dialog
    close();
}