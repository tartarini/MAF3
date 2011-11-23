/*
 *  mafDelegate.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDelegate.h"

using namespace mafCore;

mafDelegate::mafDelegate() {
}

mafDelegate::~mafDelegate() {
}

bool mafDelegate::isMethodDefined(QString signature) {
    int idx = this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(signature.toAscii()));
    return idx != -1;
}

bool mafDelegate::shouldExecuteLocalCode() {
    return true;
}
