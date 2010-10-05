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
