/*
 *  mafClientXMLRPC.cpp
 *  clientXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafClientXMLRPC.h"
#include <mafCoreDefinitions.h>
#include <mafCoreSingletons.h>

using namespace mafCore;
using namespace mafApplicationLogic;
using namespace mafEventBus;

mafClientXMLRPC::mafClientXMLRPC(): m_Address(""), m_Port(0), m_Logic(NULL) {
    mafCoreSingletons::mafSingletonsInitialize();
    m_Logic = mafNEW(mafApplicationLogic::mafLogicLight);
    m_Logic->initialize();
}

mafClientXMLRPC::~mafClientXMLRPC() {
    mafDEL(m_Logic);
    mafCoreSingletons::mafSingletonsShutdown();
}

void mafClientXMLRPC::connectToServer(unsigned int port, QString address) {
    if(m_Port == 0 || m_Port != port || m_Address.compare(address) != 0) {
        m_Port = port;
        m_Address = address;
        bool res = m_Logic->createClient("XMLRPC", m_Address, m_Port);

        QByteArray ba;
        if(res) {
            ba = mafTr("Client has been created").toAscii();
            qDebug("%s", ba.data());
        } else {
            ba = mafTr("Problem on create instance of the Client").toAscii();
            qCritical("%s", ba.data());
        }
    }
}

void mafClientXMLRPC::sendRequestToServer(mafEvent *event_dictionary, mafEventArgumentsList *listToSend) {
    m_Logic->sendRequestToServer(event_dictionary, listToSend);
}
