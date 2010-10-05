/*
 *  mafLogicLight.cpp
 *  mafApplicationLogic
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLogicLight.h"

#include <mafCoreRegistration.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafApplicationLogic;

mafLogicLight::mafLogicLight(const mafString code_location) : mafObjectBase(code_location), m_EventBus(NULL) {
}

mafLogicLight::~mafLogicLight() {
    m_EventBus->shutdown();
}

void mafLogicLight::initialize() {
    // Register all the core objects into the object factory.
    mafCoreRegistration::registerCoreObjects();

    // Initialize the event bus by asking its instance.
    m_EventBus = mafEventBusManager::instance();

}

bool mafLogicLight::startServer(const mafString &communication_protocol, unsigned int listen_port) {
    bool res(m_EventBus->createServer(communication_protocol, listen_port));
    if(res) {
        m_EventBus->startListen();
    }
    return res;
}

bool mafLogicLight::createClient(const mafString &communication_protocol, const mafString &server_host, unsigned int port) {
    bool res(m_EventBus->createClient(communication_protocol, server_host, port));
    return res;
}

void mafLogicLight::sendRequestToServer(mafEvent *event_dictionary, mafEventArgumentsList *argList) {
    //need check for event coherency
    mafEventType evType = event_dictionary->eventType();
    if(evType != mafEventTypeRemote) {
        mafMsgWarning("%s", mafTr("Wrong event type").toAscii().data());
        return;
    }
    m_EventBus->notifyEvent(*event_dictionary, argList);
}
