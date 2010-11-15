/*
 *  mafEventDispatcherRemote.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventDispatcherRemote.h"
#include "mafEvent.h"
#include "mafNetworkConnector.h"

using namespace mafEventBus;

mafEventDispatcherRemote::mafEventDispatcherRemote() : mafEventDispatcher(), m_NetworkConnectorServer(NULL), m_NetworkConnectorClient(NULL) {
    this->initializeGlobalEvents();
}

mafEventDispatcherRemote::~mafEventDispatcherRemote() {
    if(m_NetworkConnectorServer) delete m_NetworkConnectorServer;
    m_NetworkConnectorServer = NULL;
    if(m_NetworkConnectorClient) delete m_NetworkConnectorClient;
    m_NetworkConnectorClient = NULL;
}

void mafEventDispatcherRemote::initializeGlobalEvents() {
    mafEvent *properties = new mafEvent();
    mafString topic = "maf.remote.eventBus.globalUpdate";
    (*properties)[TOPIC] = topic;
    (*properties)[TYPE] = mafEventTypeRemote;
    mafVariant var;
    var.setValue((QObject*)this);
    (*properties)[OBJECT] = var;
    (*properties)[SIGTYPE] = mafSignatureTypeSignal;
    (*properties)[SIGNATURE] = "notifyDefaultEvent()";
    this->registerSignal(*properties);

    // events like remoteCommunicationDone or failed represents th bridge events between a remote communication
    // and the possibility to call local slots. The notifyEvent local sends events inside the local objects registered as observers
    // through the event bus manager while the remote notification (mafEventTypeRemote) uses the network connector.
}

void mafEventDispatcherRemote::setNetworkConnectorServer(mafNetworkConnector *connector) {
    if(m_NetworkConnectorServer == NULL) {
        m_NetworkConnectorServer = connector->clone();
        m_NetworkConnectorServer->initializeForEventBus();
    } else {
        if(m_NetworkConnectorServer->protocol() != connector->protocol()) {
            delete m_NetworkConnectorServer; //if there will be multiprotocol , here there will be a problem for thread app
            m_NetworkConnectorServer = connector->clone();
            m_NetworkConnectorServer->initializeForEventBus();
       }
   }
}

void mafEventDispatcherRemote::setNetworkConnectorClient(mafNetworkConnector *connector) {
     if(m_NetworkConnectorClient == NULL) {
         m_NetworkConnectorClient = connector->clone();
         m_NetworkConnectorClient->initializeForEventBus();
     } else {
         if(m_NetworkConnectorClient->protocol() != connector->protocol()) {
             delete m_NetworkConnectorClient; //if there will be multiprotocol , here there will be a problem for thread app
             m_NetworkConnectorClient = connector->clone();
             m_NetworkConnectorClient->initializeForEventBus();
        }
    }
}

void mafEventDispatcherRemote::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    //Q_UNUSED(event_dictionary);
    //Q_UNUSED(argList);
    Q_UNUSED(returnArg);

    // Call the notifyEventRemote converting the arguments...
    m_NetworkConnectorClient->send(event_dictionary[TOPIC].toString(), argList);
}
