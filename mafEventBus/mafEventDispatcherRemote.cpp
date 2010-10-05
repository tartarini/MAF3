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
    mafString topic = "GLOBAL_UPDATE_EVENT";
    (*properties)[TOPIC] = topic;
    (*properties)[TYPE] = mafEventTypeRemote;
    (*properties)[SIGTYPE] = mafSignatureTypeSignal;
    (*properties)[SIGNATURE] = "notifyDefaultEvent()";
    this->registerSignal(*properties);

    mafEventDispatcher::initializeGlobalEvents();
}

void mafEventDispatcherRemote::setNetworkConnectorServer(mafNetworkConnector *connector) {
    if(m_NetworkConnectorServer == NULL) {
        m_NetworkConnectorServer = connector->clone();
    } else {
        if(m_NetworkConnectorServer->protocol() != connector->protocol()) {
            delete m_NetworkConnectorServer; //if there will be multiprotocol , here there will be a problem for thread app
            m_NetworkConnectorServer = connector->clone();
       }
   }
}

void mafEventDispatcherRemote::setNetworkConnectorClient(mafNetworkConnector *connector) {
     if(m_NetworkConnectorClient == NULL) {
         m_NetworkConnectorClient = connector->clone();
     } else {
         if(m_NetworkConnectorClient->protocol() != connector->protocol()) {
             delete m_NetworkConnectorClient; //if there will be multiprotocol , here there will be a problem for thread app
             m_NetworkConnectorClient = connector->clone();
        }
    }
}

void mafEventDispatcherRemote::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    //Q_UNUSED(event_dictionary);
    //Q_UNUSED(argList);
    Q_UNUSED(returnArg);

    // Call the notifyEventRemote converting the arguments...

    mafList<mafVariant> *vl = NULL;
    if(argList != NULL) {
        vl = new mafList<mafVariant>();

        int i=0, size = argList->count();
        for(;i<size;i++) {
            mafString typeArgument;
            typeArgument = argList->at(i).name();
            if(typeArgument != "mafList<mafVariant>") {
                mafMsgWarning("%s", mafTr("Remote Dispatcher need to have arguments that are mafList<mafVariant>").toAscii().data());
                delete vl;
                return;
            }

            void *vp = argList->at(i).data();
            mafList<mafVariant> *l;
            l = (mafList<mafVariant> *)vp;
            mafVariant var;
            var.setValue(*l);

            vl->push_back(var); //only the first parameter represent the whole list of arguments
        }
        if(size == 0) {
            mafMsgWarning("%s", mafTr("Remote Dispatcher need to have at least one argument that is a mafList<mafVariant>").toAscii().data());
            return;
        }
    }

    this->notifyEventRemote(event_dictionary, vl);
    delete vl;
}

void mafEventDispatcherRemote::notifyEventRemote(const mafEvent &event_dictionary, mafList<mafVariant> *argList) const {
    //REQUIRE(m_NetworkConnectorClient != NULL);
    //mafId id_val = event_dictionary[TOPIC].toInt();
    //mafString id = mafIdProvider::instance()->idName(id_val);
    m_NetworkConnectorClient->send(event_dictionary[TOPIC].toString(), argList);
}

