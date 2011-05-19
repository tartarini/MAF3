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
#include "mafImporterWidget.h"
#include "ui_mafOperationWidget.h"
#include <mafObjectBase.h>

using namespace mafEventBus;

mafOperationWidget::mafOperationWidget(QWidget *parent) : QWidget(parent),
ui(new Ui::mafOperationWidget), m_Operation(NULL), m_OperationGUI(NULL), m_ImporterWidget(NULL) {
    ui->setupUi(this);
}

mafOperationWidget::~mafOperationWidget() {
    delete ui;
}

void mafOperationWidget::setOperation(mafCore::mafObjectBase *op) {
    m_Operation = op;
}

void mafOperationWidget::setOperationName(QString name) {
    ui->boxOperation->setTitle(name);
}

QString mafOperationWidget::operationName() const {
    return ui->boxOperation->title();
}

void mafOperationWidget::setOperationGUI(QWidget *gui) {
    QString type = m_Operation->metaObject()->className();
    
    if(type.contains("mafImporter")) {
        m_ImporterWidget = new mafImporterWidget();
        m_ImporterWidget->setOperation(m_Operation);
        m_ImporterWidget->setOperationGUI(gui);
        m_OperationGUI = m_ImporterWidget;
    } else if(type.contains("mafExporter")) {
        //TODO
    } else {
        m_OperationGUI = gui;
        mafGUI::mafConnectObjectWithGUI(m_Operation, m_OperationGUI);
    }
    
    // Connect the Ok/Cancel buttons with the GUI close.
    connect(ui->buttonOkCancel, SIGNAL(accepted()), m_OperationGUI, SLOT(close()));
    connect(ui->buttonOkCancel, SIGNAL(rejected()), m_OperationGUI, SLOT(close()));
    // and add the widget to the mafOperationWidget's layout.
    ui->verticalLayoutOperation->addWidget(m_OperationGUI);
    
}

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
    emit operationDismissed();
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
}

void mafOperationWidget::cancel() {
    emit operationDismissed();
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.stop", mafEventTypeLocal);
}
