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
    m_ComboUsername = new QComboBox( this );
    m_ComboUsername->setEditable( true );
    // initialize the password field so that it does not echo
    // characters
    m_EditPassword = new QLineEdit( this );
    m_EditPassword->setEchoMode( QLineEdit::Password );

    // initialize the labels
    m_LabelUsername = new QLabel( this );
    m_LabelPassword = new QLabel( this );
    m_LabelUsername->setText( tr( "Username" ) );
    m_LabelUsername->setBuddy( m_ComboUsername );
    m_LabelPassword->setText( tr( "Password" ) );
    m_LabelPassword->setBuddy( m_EditPassword );

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
    formGridLayout->addWidget( m_LabelUsername, 0, 0 );
    formGridLayout->addWidget( m_ComboUsername, 0, 1 );
    formGridLayout->addWidget( m_LabelPassword, 1, 0 );
    formGridLayout->addWidget( m_EditPassword, 1, 1 );
    formGridLayout->addWidget( m_Buttons, 2, 0, 1, 2 );

    setLayout( formGridLayout );
}

void mafLoginDialog::setUsername(QString &username){
    bool found = false;
    for( int i = 0; i < m_ComboUsername->count() && ! found ; i++ )
        if( m_ComboUsername->itemText( i ) == username  ){
            m_ComboUsername->setCurrentIndex( i );
            found = true;
        }

        if( ! found ){
            int index = m_ComboUsername->count();
            qDebug() << "Select username " << index;
            m_ComboUsername->addItem( username );
            m_ComboUsername->setCurrentIndex( index );
        }

        // place the focus on the password field
        m_EditPassword->setFocus();
}


void mafLoginDialog::setPassword(QString &password){
    m_EditPassword->setText( password );
}

void mafLoginDialog::slotAcceptLogin(){
    QString username = m_ComboUsername->currentText();
    QString password = m_EditPassword->text();
    int     index    = m_ComboUsername->currentIndex();

    Q_EMIT acceptLoginSignal( username,  // current username
        password,  // current password
        index      // index in the username list
        );

    // close this dialog
    close();
}

void mafGUI::mafLoginDialog::slotAbortLogin(){
    Q_EMIT abortLoginSignal();
    // close this dialog
    close();
}