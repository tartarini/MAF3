/*
 *  mafTimer.cpp
 *  mafTimeSpace
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTimer.h"
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafTimeSpace;
using namespace mafEventBus;

mafTimer::mafTimer(const mafString code_location) : mafObjectBase(code_location), m_TimerId(-1) {
    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_ObjectId = mafIdProvider::instance()->idName(objectId());
    mafRegisterLocalSignal(m_ObjectId, this, "timeoutReached()");
}

mafTimer::~mafTimer() {
    delete m_Timer;
}

void mafTimer::setInterval(int interval) {
    m_Timer->setInterval(interval);
}

void mafTimer::setSingleShot(bool one_shot) {
    m_Timer->setSingleShot(one_shot);
}

void mafTimer::start() {
    m_Timer->start();
    m_TimerId = m_Timer->timerId();
}

void mafTimer::start(int interval) {
    m_Timer->start(interval);
    m_TimerId = m_Timer->timerId();
}

void mafTimer::stop() {
    m_Timer->stop();
}

bool mafTimer::isActive() const {
    return m_Timer->isActive();
}

void mafTimer::update() {
    mafEventBusManager::instance()->notifyEvent(m_ObjectId);
}

bool mafTimer::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        if(this->m_Timer == ((mafTimer *)obj)->m_Timer &&
            this->m_TimerId == ((mafTimer *)obj)->m_TimerId) {
            return true;
        }
    }
    return false;
}
