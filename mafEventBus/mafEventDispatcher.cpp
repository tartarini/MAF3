/*
 *  mafEventDispatcher.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafEventDispatcher.h"
#include "mafEvent.h"

#define CALLBACK_SIGNATURE "1"
#define SIGNAL_SIGNATURE   "2"

using namespace mafEventBus;

mafEventDispatcher::mafEventDispatcher() {

}

mafEventDispatcher::~mafEventDispatcher() {
    // delete all lists present into the hash.
    mafHash<mafString, mafEvent *>::iterator i;
    for (i = m_CallbacksHash.begin(); i != m_CallbacksHash.end(); ++i) {
        delete i.value();
    }
    m_CallbacksHash.clear();

    for (i = m_SignalsHash.begin(); i != m_SignalsHash.end(); ++i) {
        delete i.value();
    }
    m_SignalsHash.clear();
}

void mafEventDispatcher::initializeGlobalEvents() {
    mafEvent *remote_done = new mafEvent();
    mafString eventId = "maf.remote.eventBus.comunicationDone";

    (*remote_done)[TOPIC] = eventId;
    (*remote_done)[TYPE] = mafEventTypeLocal;
    (*remote_done)[SIGTYPE] = mafSignatureTypeSignal;
    mafVariant var;
    var.setValue((QObject*)this);
    (*remote_done)[OBJECT] = var;
    (*remote_done)[SIGNATURE] = "remoteCommunicationDone()";
    this->registerSignal(*remote_done);

    mafEvent *remote_failed = new mafEvent();
    (*remote_failed)[TOPIC] = "maf.remote.eventBus.comunicationFalied";
    (*remote_failed)[TYPE] = mafEventTypeLocal;
    (*remote_failed)[SIGTYPE] = mafSignatureTypeSignal;
    var.setValue((QObject*)this);
    (*remote_failed)[OBJECT] = var;
    (*remote_failed)[SIGNATURE] = "remoteCommunicationFailed()";
    this->registerSignal(*remote_failed);
}

bool mafEventDispatcher::isSignaturePresent(const mafEvent &props) const {
    mafString topic = props[TOPIC].toString();
    mafEventItemListType itemEventPropList;
    mafEvent *itemEventProp;
    if(props[SIGTYPE].toInt() == mafSignatureTypeCallback) {
        itemEventPropList = m_CallbacksHash.values(topic);
    } else {
        itemEventPropList = m_SignalsHash.values(topic);
    }

    QObject *objParameter = props[OBJECT].value<QObject *>();
    foreach(itemEventProp, itemEventPropList) {
        QObject *objInList = (*itemEventProp)[OBJECT].value<QObject *>();
        if(objInList == objParameter && (*itemEventProp)[SIGNATURE].toString() == props[SIGNATURE].toString()) {
            return true;
        }
    }
    return false;
}

bool mafEventDispatcher::disconnectSignal(const mafEvent &props) {
    QObject *obj_signal = props[OBJECT].value<QObject*>();
    mafString sig = props[SIGNATURE].toString();
    mafString event_sig = SIGNAL_SIGNATURE;
    event_sig.append(sig);
    bool result = obj_signal->disconnect(obj_signal, event_sig.toAscii(), 0, 0);
    return result;
}

bool mafEventDispatcher::disconnectCallback(const mafEvent &props) {
    //need to disconnect observer from the signal
    mafString observer_sig = CALLBACK_SIGNATURE;
    observer_sig.append(props[SIGNATURE].toString());

    mafEvent *itemSignal = m_SignalsHash.value(props[TOPIC].toString());
    mafString event_sig = SIGNAL_SIGNATURE;
    event_sig.append((*itemSignal)[SIGNATURE].toString());

    QObject *objSignal = (*itemSignal)[OBJECT].value<QObject *>();
    QObject *objSlot = props[OBJECT].value<QObject *>();

    return disconnect(objSignal, event_sig.toAscii(), objSlot, observer_sig.toAscii());
}

bool mafEventDispatcher::removeEventItem(const mafEvent &props) {
    bool isDisconnected = false;
    bool isPresent = isSignaturePresent(props);
    if(isPresent == true) {
        //mafEventItemListType itemEventPropList;
        if(props[SIGTYPE].toInt() == mafSignatureTypeCallback) {
            isDisconnected = disconnectCallback(props);
            //iterator for erasing hash entry
            mafEventsHashType::iterator i = m_CallbacksHash.find(props[TOPIC].toString());
            while (i != m_CallbacksHash.end() && i.key() == props[TOPIC]) {
                QObject *obj = (*(i.value()))[OBJECT].value<QObject *>();
                QObject *objCheck = props[OBJECT].value<QObject *>();
                if (obj == objCheck && (*(i.value()))[SIGNATURE].toString() == props[SIGNATURE].toString()) {
                    delete i.value();
                    i = m_CallbacksHash.erase(i);
                } else {
                    ++i;
                }
            }
        } else {
            //itemEventPropList = m_SignalsHash.values();
            isDisconnected = disconnectSignal(props);
            mafEventsHashType::iterator i = m_CallbacksHash.find(props[TOPIC].toString());
            while (i != m_CallbacksHash.end() && i.key() == props[TOPIC].toString()) {
                delete i.value();
                i++;
            }
            i = m_SignalsHash.find(props[TOPIC].toString());
            while (i != m_SignalsHash.end() && i.key() == props[TOPIC].toString()) {
                delete i.value();
                i++;
            }
            m_SignalsHash.remove(props[TOPIC].toString()); //in signal hash the id is unique
            m_CallbacksHash.remove(props[TOPIC].toString()); //remove also all the id associated in callback
        }

        //itemEventPropList.removeAt(idx);
    }
    return isDisconnected;
}

bool mafEventDispatcher::addObserver(const mafEvent &props) {
    mafString topic = props[TOPIC].toString();
    // check if the object has been already registered with the same signature to avoid duplicates.
    if(m_CallbacksHash.contains(topic) && this->isSignaturePresent(props) == true) {
        return false;
    }

    mafEvent *itemEventProp;
    itemEventProp = m_SignalsHash.value(topic);
    if(itemEventProp == NULL) {
        mafMsgDebug() << mafTr("Signal not present for topic %1").arg(topic);
        return false;
    }
    mafVariant sigVariant = (*itemEventProp)[SIGNATURE];
    mafString sig = sigVariant.toString();
    if(sig.length() > 0) {
        mafString observer_sig = CALLBACK_SIGNATURE;
        observer_sig.append(props[SIGNATURE].toString());

        mafString event_sig = SIGNAL_SIGNATURE;
        event_sig.append(sig);

        // Add the new observer to the Hash.
        mafEvent *dict = const_cast<mafEvent *>(&props);
        this->m_CallbacksHash.insertMulti(topic, dict);
        QObject *objSignal = (*itemEventProp)[OBJECT].value<QObject *>();
        QObject *objSlot = props[OBJECT].value<QObject *>();
        return connect(objSignal, event_sig.toAscii(), objSlot, observer_sig.toAscii());
    }
    return false;
}

bool mafEventDispatcher::removeObserver(const QObject *obj, const mafString topic, bool qt_disconnect) {
    if(obj == NULL) {
        return false;
    }

    return removeFromHash(&m_CallbacksHash, obj, topic, qt_disconnect);
}

bool mafEventDispatcher::removeSignal(const QObject *obj, const mafString topic, bool qt_disconnect) {
    if(obj == NULL) {
        return false;
    }

    return removeFromHash(&m_SignalsHash, obj, topic, qt_disconnect);
}

bool mafEventDispatcher::removeFromHash(mafEventsHashType *hash, const QObject *obj, const mafString topic, bool qt_disconnect) {
    bool disconnectItem = true;
    if(topic.length() > 0 && hash->contains(topic)) {
        // Remove the observer from the given topic.
        mafEventsHashType::iterator i = hash->find(topic);
        while(i != hash->end() && i.key() == topic) {
            QObject *item = (*(i.value()))[OBJECT].value<QObject *>();
            if(item == obj) {
                mafEvent *prop = i.value();
                bool currentDisconnetFlag = false;
                if(qt_disconnect) {
                    if(*hash == m_CallbacksHash) {
                        currentDisconnetFlag = disconnectCallback(*prop);
                    } else {
                        currentDisconnetFlag = disconnectSignal(*prop);
                    }
                } else {
                    currentDisconnetFlag = true;
                }
                disconnectItem = disconnectItem && currentDisconnetFlag;
                if(currentDisconnetFlag) {
                    delete i.value();
                    i = hash->erase(i);
                } else {
                    qDebug() << mafTr("Unable to disconnect object %1 on topic %2").arg(obj->objectName(), topic);
                    ++i;
                }
            } else {
                ++i;
            }
        }
        return disconnectItem;
    }

    if(topic.isEmpty()) {
        mafEventsHashType::iterator i = hash->begin();
        while(i != hash->end()) {
            QObject *item = (*(i.value()))[OBJECT].value<QObject *>();
            if(item == obj) {
                mafEvent *prop = i.value();
                bool currentDisconnetFlag = false;
                if(qt_disconnect) {
                    if(*hash == m_CallbacksHash) {
                        currentDisconnetFlag = disconnectCallback(*prop);
                    } else {
                        currentDisconnetFlag = disconnectSignal(*prop);
                    }
                } else {
                    currentDisconnetFlag = true;
                }
                disconnectItem = disconnectItem && currentDisconnetFlag;
                if(currentDisconnetFlag) {
                    delete i.value();
                    i = hash->erase(i);
                } else {
                    qDebug() << mafTr("Unable to disconnect object %1 from topic %2").arg(obj->objectName(), (*prop)[TOPIC].toString());
                    ++i;
                }
            } else {
                ++i;
            }
        }
        return disconnectItem;
    }

    return false; //need to enter in one of the conditions
}

bool mafEventDispatcher::removeObserver(const mafEvent &props) {
    QObject *objSlot = props[OBJECT].value<QObject *>();
    if (objSlot == NULL) {
        // remove all observer for that 'id'
        bool result = disconnectSignal(props);

        int num = m_CallbacksHash.remove(props[TOPIC].toString());
        return result && num > 0;
    }

    return removeEventItem(props);
}

bool mafEventDispatcher::registerSignal(const mafEvent &props) {
    // check if the object has been already registered with the same signature to avoid duplicates.
    if(props["Signature"].toString().length() == 0) {
        QVariant var;
        var.setValue((QObject *)this);
        props[OBJECT] = var;
        props[SIGNATURE] = "notifyDefaultEvent()";
    }

    mafString topic = props[TOPIC].toString();
    // Check if a signal (corresponding to a mafID) already is present.
    if(m_SignalsHash.contains(topic)) {// && (this->isSignaturePresent(signal_props) == true)) {
        // Only one signal for a given id can be registered!!
        QObject *obj = props[OBJECT].value<QObject *>();
        if(obj != NULL) {
            mafMsgWarning("%s", mafTr("Object %1 is trying to register a signal with ID '%2' that has been already registered!!").arg(obj->metaObject()->className(), topic).toAscii().data());
        } else {
            mafMsgWarning("%s", mafTr("NULL is trying to register a signal with ID '%2' that has been already registered!!").arg(topic).toAscii().data());
        }
        return false;
    }

    // Add the new signal to the Hash.
    mafEvent *dict = const_cast<mafEvent *>(&props);
    this->m_SignalsHash.insert(topic, dict);
    return true;
}

bool mafEventDispatcher::removeSignal(const mafEvent &props) {
    QObject *obj = props[OBJECT].value<QObject *>();
    if(obj == NULL) {
        // Remove all events corresponding to particular id
        int num = m_SignalsHash.remove(props[TOPIC].toString());
        return num > 0;
    }

    return removeEventItem(props);
}

void mafEventDispatcher::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    Q_UNUSED(event_dictionary);
    Q_UNUSED(argList);
    Q_UNUSED(returnArg);
}
