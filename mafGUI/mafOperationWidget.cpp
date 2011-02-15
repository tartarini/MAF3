/*
 *  mafOperationWidget.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 08/02/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationWidget.h"
#include "ui_mafOperationWidget.h"
#include <mafObjectBase.h>

using namespace mafEventBus;

mafOperationWidget::mafOperationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mafOperationWidget), m_Operation(NULL), m_OperationGUI(NULL) {
    ui->setupUi(this);

    this->setLayout(ui->verticalLayout);
    ui->boxOperation->setLayout(ui->verticalLayoutBox);
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayoutOperation);
    ui->scrollAreaWidgetContents->setBackgroundRole(QPalette::NoRole);
}

mafOperationWidget::~mafOperationWidget() {
    delete ui;
}

void mafOperationWidget::setOperation(mafCore::mafObjectBase *op) {
    m_Operation = op;
}

void mafOperationWidget::setOperationName(mafString name) {
    ui->boxOperation->setTitle(name);
}

void mafOperationWidget::setOperationGUI(QWidget *gui) {
    m_OperationGUI = gui;
    connect(ui->buttonOkCancel, SIGNAL(accepted()), m_OperationGUI, SLOT(close()));
    connect(ui->buttonOkCancel, SIGNAL(rejected()), m_OperationGUI, SLOT(close()));
    ui->verticalLayoutOperation->addWidget(m_OperationGUI);
    (m_Operation)->connectObjectSlotsByName((QObject *)m_OperationGUI);
    (m_Operation)->initializeUI((QObject *)m_OperationGUI);
}

/*void mafOperationWidget::setParameter(QVariant var) {
    QString name = QObject::sender()->objectName();
//    m_Operation->setProperty(name, var);
}*/

void mafOperationWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void mafOperationWidget::execute() {
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    emit operationDismissed();
}

void mafOperationWidget::cancel() {
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.stop", mafEventTypeLocal);
    emit operationDismissed();
}
