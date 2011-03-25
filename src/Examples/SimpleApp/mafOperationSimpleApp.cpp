/*
 *  mafOperationSimpleApp.cpp
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationSimpleApp.h"

using namespace mafResources;

mafOperationSimpleApp::mafOperationSimpleApp(const QString code_location) : mafOperation(code_location), m_CheckScalar(false) {
    m_UIFilename = "mafOperationSimpleApp.ui";
    m_CanUnDo = false;
    setObjectName("mafOperationSimpleApp");
}

bool mafOperationSimpleApp::acceptObject(mafCore::mafObjectBase *obj) {
    return true;
    if(obj == NULL) {
        return false;
    }
    QString ct(obj->metaObject()->className());
    return ct == "mafResources::mafVME";
}

void mafOperationSimpleApp::setCheckScalar(bool s) {
    qDebug() << mafTr("Assigning scalar...");
    m_CheckScalar = s;
}

void mafOperationSimpleApp::execute() {
    qDebug() << mafTr("Executing demo operation...");
}

void mafOperationSimpleApp::on_textTest_textEdited(QString text) {
    qDebug() << "Text inserted: " << text;
}

void mafOperationSimpleApp::on_checkScalar_clicked(bool state) {
    qDebug() << "Scalar State: " << state;
}
