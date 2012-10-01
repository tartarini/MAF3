/*
*  mafLoginDialog.cpp
*  mafGUI
*
*  Created by Roberto Mucci on 26/10/10.
*  Copyright 2010 SCS-B3C. All rights reserved.
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
    //Set progress bar
    m_Progress = new QProgressDialog("Waiting for authentication...", "Cancel", 0, 0, this);
    m_Progress->setWindowTitle(tr("Login"));

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
    readSettings();
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
    writeSettings();
    QString username = m_EditUsername->text();
    QString password = m_EditPassword->text();
    bool remember = m_Checkbox->isChecked();
    Q_EMIT acceptLoginSignal( username, password, remember );
}

void mafLoginDialog::slotAbortLogin(){
    Q_EMIT abortLoginSignal();
}

void mafLoginDialog::writeSettings(void) {
    QSettings settings;
    if (m_Checkbox->isChecked()) {
        //write user credential
        QSettings settings;
        settings.setValue("username", m_EditUsername->text());
        settings.setValue("password", m_EditPassword->text());
        settings.setValue("remember", m_Checkbox->isChecked());
    } else {
        //reset user credential
        QSettings settings;
        settings.setValue("username", "");
        settings.setValue("password", "");
        settings.setValue("remember", m_Checkbox->isChecked());
    }
    settings.sync();
}

void mafLoginDialog::readSettings(void) {
    QSettings settings;
    bool remember = settings.value("remember").toBool();
    QString user = settings.value("username").toString();
    if (remember) {
        m_EditUsername->setText(settings.value("username").toString());
        m_EditPassword->setText(settings.value("password").toString());
        m_Checkbox->setChecked(remember);
    }
}

void mafGUI::mafLoginDialog::showProgressBar(){
    m_Progress->show();
}

void mafGUI::mafLoginDialog::closeProgressBar(){
    m_Progress->close();
}