/*
 *  mafNetworkConnectorREST.cpp
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNetworkConnectorREST.h"
#include "mafEventBusManager.h"

#include "mafEvent.h"

using namespace mafEventBus;

mafNetworkConnectorREST::mafNetworkConnectorREST() : mafNetworkConnector() {
    m_Protocol = "REST";
}

void mafNetworkConnectorREST::initializeForEventBus() {
    //mafRegisterRemoteSignal("maf.remote.eventBus.communication.send.REST", this, "remoteCommunication(const QString, mafEventArgumentsList *)");
    //mafRegisterRemoteCallback("maf.remote.eventBus.communication.send.REST", this, "send(const QString, mafEventArgumentsList *)");
}

mafNetworkConnectorREST::~mafNetworkConnectorREST() {
}

mafNetworkConnector *mafNetworkConnectorREST::clone() {
    mafNetworkConnectorREST *copy = new mafNetworkConnectorREST();
    return copy;
}

void mafNetworkConnectorREST::createClient(const QString hostName, const unsigned int port) {
    bool result(false);
}

void mafNetworkConnectorREST::createServer(const unsigned int port) {
}

void mafNetworkConnectorREST::startListen() {
}

void mafNetworkConnectorREST::send(const QString event_id, mafEventArgumentsList *argList) {
/*    QList<xmlrpc::Variant> *vl = NULL;
    if(argList != NULL) {
        vl = new QList<xmlrpc::Variant>();

        int i=0, size = argList->count();
        for(;i<size;++i) {
            QString typeArgument;
            typeArgument = argList->at(i).name();
            if(typeArgument != "QVariantList") {
                qWarning("%s", mafTr("Remote Dispatcher need to have arguments that are QVariantList").toAscii().data());
                delete vl;
                return;
            }

            void *vp = argList->at(i).data();
            QVariantList *l;
            l = (QVariantList *)vp;
            xmlrpc::Variant var;
            var.setValue(*l);

            vl->push_back(var); //only the first parameter represent the whole list of arguments
        }
        if(size == 0) {
            qWarning("%s", mafTr("Remote Dispatcher need to have at least one argument that is a QVariantList").toAscii().data());
            return;
        }
    }

   xmlrpcSend(event_id, *vl);
   delete vl;*/
}