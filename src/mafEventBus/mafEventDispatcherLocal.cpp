/*
 *  mafEventDispatcherLocal.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventDispatcherLocal.h"
#include "mafEvent.h"

using namespace mafEventBus;

mafEventDispatcherLocal::mafEventDispatcherLocal() : mafEventDispatcher() {
    this->initializeGlobalEvents();
}

void mafEventDispatcherLocal::initializeGlobalEvents() {
    mafEvent *properties = new mafEvent();
    QString topic = "maf.local.eventBus.globalUpdate";
    (*properties)[TOPIC] =  topic;
    (*properties)[TYPE] = mafEventTypeLocal;
    (*properties)[SIGTYPE] = mafSignatureTypeSignal;
    QVariant var;
    var.setValue((QObject*)this);
    (*properties)[OBJECT] = var;
    (*properties)[SIGNATURE] = "notifyDefaultEvent()";
    registerSignal(*properties);

    mafEventDispatcher::initializeGlobalEvents();
}

void mafEventDispatcherLocal::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, QGenericReturnArgument *returnArg) const {
    QString topic = event_dictionary[TOPIC].toString();
    mafEventItemListType items = signalItemProperty(topic);
    mafEvent *itemEventProp;

    mafEventArgumentsListPointer argListPointer = event_dictionary.argumentList();
    bool hasArgs(argListPointer->count() > 0);
    if (hasArgs) {
        // We have arguments attached to the event...
        argList = argListPointer;
    }
    const QGenericReturnArgument *retVal = event_dictionary.returnValue();
    if (retVal && retVal->data() != NULL) {
        returnArg = (QGenericReturnArgument *)retVal;
    }
    
    Qt::ConnectionType connType = event_dictionary.isSynchronous() ? Qt::DirectConnection : Qt::AutoConnection;

    Q_FOREACH(itemEventProp, items) {
        if((*itemEventProp)[SIGNATURE].toString().length() != 0) {
            QString signal_to_emit = (*itemEventProp)[SIGNATURE].toString().split("(")[0];
            QObject *obj = (*itemEventProp)[OBJECT].value<QObject *>();
            if(argList != NULL) {
                if (returnArg == NULL || returnArg->data() == NULL) { //don't use return value
                    switch (argList->count()) {
                        case 0:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType);
                            break;
                        case 1:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                            connType, argList->at(0));
                            break;
                        case 2:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                            connType, argList->at(0), argList->at(1));
                            break;
                        case 3:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                            connType, argList->at(0), argList->at(1), argList->at(2));
                            break;
                        case 4:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3));
                            break;
                        case 5:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4));
                            break;
                        case 6:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5));
                            break;
                        case 7:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6));
                            break;
                        case 8:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7));
                            break;
                        case 9:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8));
                            break;
                        case 10:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             connType, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8), argList->at(9));
                            break;
                        default: {
                            QByteArray ba = mafTr("Number of arguments not supported. Max 10 arguments").toAscii();
                            qWarning("%s", ba.data());
                        }
                    } //switch
                 } else { //use return value
                    switch (argList->count()) {
                        case 0:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType, *returnArg);
                            break;
                        case 1:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0));
                            break;
                        case 2:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                             *returnArg, argList->at(0), argList->at(1));
                            break;
                        case 3:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0), argList->at(1), argList->at(2));
                            break;
                        case 4:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                             *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3));
                            break;
                        case 5:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4));
                            break;
                        case 6:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5));
                            break;
                        case 7:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6));
                            break;
                        case 8:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                             *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7));
                            break;
                        case 9:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8));
                            break;
                        case 10:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType,\
                            *returnArg, argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                            argList->at(5), argList->at(6), argList->at(7), argList->at(8), argList->at(9));
                            break;
                        default: {
                            QByteArray ba = mafTr("Number of arguments not supported. Max 10 arguments").toAscii();
                            qWarning("%s", ba.data());
                        }
                    } //switch
                 }
            } else {
                if (returnArg == NULL || returnArg->data() == NULL) { //don't use return value
                    this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType);
                } else {
                    this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), connType, *returnArg);
                }
            }
        }
    }
}

