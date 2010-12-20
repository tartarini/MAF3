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

mafOperationSimpleApp::mafOperationSimpleApp(const mafString code_location) : mafOperation(code_location) {
    m_UIFilename = "mafOperationSimpleApp.ui";
}

bool mafOperationSimpleApp::acceptObject(mafCore::mafObjectBase *obj) {
    if(obj == NULL) {
        return false;
    }
    mafString ct(obj->metaObject()->className());
//    mafObject *vme = dynamic_cast<mafObject*>(obj);
    return ct == "mafResources::mafVME";
}
