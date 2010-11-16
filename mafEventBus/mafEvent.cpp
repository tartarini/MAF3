/*
 *  mafEvent.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEvent.h"

using namespace mafEventBus;

mafEvent::mafEvent() {
    m_EventHash = new mafEventHash();
}

/// Overload object constructor.
mafEvent::mafEvent(mafString topic, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, mafString signature) {
    m_EventHash = new mafEventHash();
    entries()->insert(TOPIC, topic);
    entries()->insert(TYPE, static_cast<int>(event_type));
    entries()->insert(SIGTYPE, static_cast<int>(signature_type));
    mafVariant var;
    var.setValue(objectPointer);
    entries()->insert(OBJECT, var);
    entries()->insert(SIGNATURE, signature);
}

mafEvent::~mafEvent() {
    m_EventHash->clear();
    delete m_EventHash;
}

mafEventHash *mafEvent::entries() {
    return m_EventHash;
}

mafEventHash *mafEvent::entries() const {
    return m_EventHash;
}

mafVariant &mafEvent::operator[](mafString key) const{
    return (*m_EventHash)[key];
}

mafEventType mafEvent::eventType() const {
    return static_cast<mafEventType>(entries()->value(TYPE).toInt());
}

mafString mafEvent::eventTopic() const {
    return entries()->value(TOPIC).toString();
}

bool mafEvent::isEventLocal() const {
    int et = entries()->value(TYPE).toInt();
    return et == mafEventTypeLocal;
}

void mafEvent::setEventType(mafEventType et) {
    entries()->insert(TYPE, static_cast<int>(et));
}

void mafEvent::setEventTopic(mafString topic) {
    entries()->insert(TOPIC, topic);
}

/*mafString mafEvent::eventIdName() const {
    mafId id = eventId();
    return mafIdProvider::instance()->idName(id);
}*/
