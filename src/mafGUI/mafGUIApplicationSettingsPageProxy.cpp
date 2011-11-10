/*
 *  mafGUIApplicationSettingsPageProxy.cpp
 *  mafGUI
 *
 *  Created by Robero Mucci on 02/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUIApplicationSettingsPageProxy.h"

#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>


using namespace mafGUI;

mafGUIApplicationSettingsPageProxy::mafGUIApplicationSettingsPageProxy(QWidget *parent) : mafGUIApplicationSettingsPage(parent) {
    m_PageText = mafTr("Proxy");
    m_PageIcon = QIcon(":/images/config.png");

    // Build the Page's GUI
    QGroupBox *configGroup = new QGroupBox(tr("Proxy configuration"));

    QLabel *proxyHostLabel = new QLabel(tr("Proxy host:"));
    m_ProxyHostLineEdit = new QLineEdit();
    m_ProxyHostLineEdit->setObjectName(QString::fromUtf8("proxyHostLineEdit"));

    QLabel *proxyPortLabel = new QLabel(tr("Proxy port:"));
    m_ProxyPortLineEdit = new QLineEdit();
    m_ProxyPortLineEdit->setValidator(new QIntValidator(this));
    m_ProxyPortLineEdit->setObjectName(QString::fromUtf8("proxyPortLineEdit"));

    QHBoxLayout *proxyHostLayout = new QHBoxLayout;
    proxyHostLayout->addWidget(proxyHostLabel);
    proxyHostLayout->addWidget(m_ProxyHostLineEdit);

    QHBoxLayout *proxyPortLayout = new QHBoxLayout;
    proxyPortLayout->addWidget(proxyPortLabel);
    proxyPortLayout->addWidget(m_ProxyPortLineEdit);

   
    QHBoxLayout *useProxyLayout = new QHBoxLayout;
    QLabel *useProxyLabel = new QLabel( this );
    useProxyLabel->setText(tr("use proxy"));
    m_Checkbox = new QCheckBox();

    // connects slots
    connect(m_Checkbox, SIGNAL(stateChanged(int)), this, SLOT(slotCheckBox(int)));
    connect(m_ProxyHostLineEdit, SIGNAL(editingFinished()), this, SLOT(writeSettings(void)));
    connect(m_ProxyPortLineEdit, SIGNAL(editingFinished()), this, SLOT(writeSettings(void)));

    useProxyLayout->addWidget(useProxyLabel);
    useProxyLayout->addWidget(m_Checkbox);
    useProxyLayout->addStretch(1);


    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addLayout(proxyHostLayout);
    configLayout->addLayout(proxyPortLayout);
    configLayout->addLayout(useProxyLayout);
    configGroup->setLayout(configLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    readSettings();
}

void mafGUIApplicationSettingsPageProxy::writeSettings(void) {
    QSettings settings;
    settings.beginGroup("proxy");
    settings.setValue("useproxy", m_Checkbox->isChecked());
    settings.setValue("proxyhost", m_ProxyHostLineEdit->text());
    settings.setValue("proxyport", m_ProxyPortLineEdit->text());
    settings.endGroup();
    settings.sync();
}

void mafGUIApplicationSettingsPageProxy::readSettings(void) {
    QSettings settings;
    settings.beginGroup("proxy");
    m_ProxyHostLineEdit->setText(settings.value("proxyhost").toString());
    m_ProxyPortLineEdit->setText(settings.value("proxyport").toString());
    bool useProxy = settings.value("useproxy").toBool();
    m_Checkbox->setChecked(useProxy);
    settings.endGroup();
}

void mafGUI::mafGUIApplicationSettingsPageProxy::slotCheckBox(int state){
    m_ProxyHostLineEdit->setEnabled(state);
    m_ProxyPortLineEdit->setEnabled(state);
    writeSettings();
}
