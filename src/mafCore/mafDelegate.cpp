/*
 *  mafDelegate.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDelegate.h"

using namespace mafCore;

mafDelegate::mafDelegate() : mafReferenceCounted() {
}

mafDelegate::~mafDelegate() {
}

bool mafDelegate::isMethodDefined(QString signature) {
    int idx = this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(signature.toLatin1()));
    return idx != -1;
}

bool mafDelegate::shouldExecuteLocalCode() {
    return true;
}

void mafDelegate::executeMethod(QString signature, mafArgumentList *argList /* = NULL */, QGenericReturnArgument *ret /* = NULL */) const {
    QString method = signature.split("(").at(0);
    if (signature.indexOf("(") == -1) {
        qWarning() << mafTr("Bad method signature: ").arg(signature);
        return;
    }


    if (ret == NULL) {
        // return value is NULL => void
        if (argList == NULL || argList->count() == 0) {
            // Method with neither arguments.
            metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection);
        } else {
            // Method with arguments.
            switch (argList->count()) {
                case 1:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0));
                    break;
                case 2:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1));
                    break;
                case 3:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2));
                    break;
                case 4:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3));
                    break;
                case 5:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4));
                    break;
                case 6:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5));
                    break;
                case 7:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6));
                    break;
                case 8:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6), argList->at(7));
                    break;
                case 9:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6), argList->at(7), argList->at(8));
                    break;
                case 10:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), \
                        Qt::DirectConnection, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6), argList->at(7), argList->at(8), argList->at(9));
                    break;
            }
        }
    } else {
        // delegate method has return value.
        if (argList == NULL || argList->count() == 0) {
            // Method has no arguments.
            metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection, *ret);
        } else {
            // Method has also arguments.
            switch (argList->count()) {
                case 1:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection, *ret, argList->at(0));
                    break;
                case 2:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1));
                    break;
                case 3:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2));
                    break;
                case 4:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3));
                    break;
                case 5:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4));
                    break;
                case 6:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5));
                    break;
                case 7:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6));
                    break;
                case 8:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6), argList->at(7));
                    break;
                case 9:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6), argList->at(7), argList->at(8));
                    break;
                case 10:
                    metaObject()->invokeMethod((QObject *)this, method.toLatin1(), Qt::DirectConnection,\
                        *ret, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5), argList->at(6), argList->at(7), argList->at(8), argList->at(9));
                    break;
            }
        }
    }
}
