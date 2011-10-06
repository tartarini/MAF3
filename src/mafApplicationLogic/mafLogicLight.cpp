/*
 *  mafLogicLight.cpp
 *  mafApplicationLogic
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLogicLight.h"

#include <mafCoreSingletons.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafApplicationLogic;

mafLogicLight::mafLogicLight(const QString code_location) : mafObjectBase(code_location), m_EventBus(NULL) {
}

mafLogicLight::~mafLogicLight() {
    m_EventBus->shutdown();
    mafCoreSingletons::mafSingletonsShutdown();
}

bool mafLogicLight::initialize() {
    // Register all the core objects into the object factory.
    mafCoreSingletons::mafSingletonsInitialize();

    // Initialize the event bus by asking its instance.
    m_EventBus = mafEventBusManager::instance();

    return m_EventBus != NULL;
}

bool mafLogicLight::startServer(const QString &communication_protocol, unsigned int listen_port) {
    bool res(m_EventBus->createServer(communication_protocol, listen_port));
    if(res) {
        m_EventBus->startListen();
    }
    return res;
}

bool mafLogicLight::createClient(const QString &communication_protocol, const QString &server_host, unsigned int port) {
    bool res(m_EventBus->createClient(communication_protocol, server_host, port));
    return res;
}

void mafLogicLight::sendRequestToServer(mafEvent *event_dictionary, mafEventArgumentsList *argList) {
    //need check for event coherency
    mafEventType evType = event_dictionary->eventType();
    if(evType != mafEventTypeRemote) {
        QByteArray ba = mafTr("Wrong event type").toAscii();
        qWarning("%s", ba.data());
        return;
    }
    m_EventBus->notifyEvent(*event_dictionary, argList);
}
