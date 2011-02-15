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

#define CALLBACK_SIGNATURE "1"
#define SIGNAL_SIGNATURE   "2"

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

void mafOperationWidget::setOperation(QObject *op) {
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
    this->connectOperationSlotsByName(m_Operation, (QObject *)m_OperationGUI);
}

void mafOperationWidget::connectOperationSlotsByName(QObject *slot_object, QObject *signal_object) {
    if (!slot_object)
        return;
    const QMetaObject *mo = slot_object->metaObject();
    Q_ASSERT(mo);
    const QObjectList list = qFindChildren<QObject *>(signal_object, QString());
    for (int i = 0; i < mo->methodCount(); ++i) {
        QMetaMethod method_slot = mo->method(i);
        if (method_slot.methodType() != QMetaMethod::Slot)
            continue;
        const char *slot = mo->method(i).signature();
        if (slot[0] != 'o' || slot[1] != 'n' || slot[2] != '_')
            continue;
        bool foundIt = false;
        for(int j = 0; j < list.count(); ++j) {
            const QObject *co = list.at(j);
            QByteArray objName = co->objectName().toAscii();
            int len = objName.length();
            if (!len || qstrncmp(slot + 3, objName.data(), len) || slot[len+3] != '_')
                continue;
            int sigIndex = -1; //co->metaObject()->signalIndex(slot + len + 4);
            const QMetaObject *smo = co->metaObject();
            if (sigIndex < 0) { // search for compatible signals
                int slotlen = qstrlen(slot + len + 4) - 1;
                for (int k = 0; k < co->metaObject()->methodCount(); ++k) {
                    QMetaMethod method = smo->method(k);
                    if (method.methodType() != QMetaMethod::Signal)
                        continue;

                    if (!qstrncmp(method.signature(), slot + len + 4, slotlen)) {
                        const char *signal = method.signature();
                        mafString event_sig = SIGNAL_SIGNATURE;
                        event_sig.append(signal);

                        mafString observer_sig = CALLBACK_SIGNATURE;
                        observer_sig.append(slot);

                        if(connect(co, event_sig.toAscii(), slot_object, observer_sig.toAscii())) {
                            mafMsgDebug() << mafTr("CONNECTED slot %1 with signal %2").arg(slot, signal);
                            foundIt = true;
                            break;
                        } else {
                            mafMsgWarning() << mafTr("Cannot connect slot %1 with signal %2").arg(slot, signal);
                        }
                    }
                }
            }
        }
        if (foundIt) {
            // we found our slot, now skip all overloads
            while (mo->method(i + 1).attributes() & QMetaMethod::Cloned)
                  ++i;
        } else if (!(mo->method(i).attributes() & QMetaMethod::Cloned)) {
            qWarning("QMetaObject::connectSlotsByName: No matching signal for %s", slot);
        }
    }
}

void mafOperationWidget::setParameter(QVariant var) {
    QString name = QObject::sender()->objectName();
//    m_Operation->setProperty(name, var);
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
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    emit operationDismissed();
}

void mafOperationWidget::cancel() {
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.stop", mafEventTypeLocal);
    emit operationDismissed();
}
