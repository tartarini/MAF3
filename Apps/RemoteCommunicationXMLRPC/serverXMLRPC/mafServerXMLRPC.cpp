/*
 *  mafServerXMLRPC.cpp
 *  serverXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafServerXMLRPC.h"
#include "mafAlgorithm.h"
#include <mafCoreDefinitions.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafOperationManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafApplicationLogic;
using namespace mafResources;

mafServerXMLRPC::mafServerXMLRPC() : m_Port(0), m_Logic(NULL) {
    mafCoreSingletons::mafSingletonsInitialize();
    m_Logic = mafNEW(mafApplicationLogic::mafLogic);
    m_Logic->initialize();
    mafRegisterObject(mafAlgorithm);

}

mafServerXMLRPC::~mafServerXMLRPC() {
    mafUnregisterObject(mafAlgorithm);
    mafDEL(m_Logic);
    mafCoreSingletons::mafSingletonsShutdown();
}

void mafServerXMLRPC::startListen(unsigned int port) {
    if(m_Port == 0) {
        m_Port = port;
        bool res = m_Logic->startServer("XMLRPC", port);
        if(res) {
            qDebug() << mafTr("Server listening on port %1").arg(m_Port);
        } else {
            qCritical() << mafTr("Server cannot start listen at port: %1").arg(m_Port);
        }
    } else {
        qDebug() << mafTr("Server already running on port %1").arg(m_Port);
    }
}
