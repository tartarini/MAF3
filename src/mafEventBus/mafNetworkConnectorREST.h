/*
 *  mafNetworkConnectorREST.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNETWORKCONNECTORREST_H
#define MAFNETWORKCONNECTORREST_H

// include list
#include "mafNetworkConnector.h"

// Foundation Library include list
#include <client.h>
#include <server.h>

namespace mafEventBus {

/**
 Class name: mafNetworkConnectorREST
 This class is the implementation class for client/server objects that works over network
 with xml-rpc protocol. The server side part also create a new ID named REGISTER_SERVER_METHODS_XXX
 (where the XXX is the port on which run the server) that allows you to register your own remote
 callbacks. The library used is qxmlrpc.
 */
class MAFEVENTBUSSHARED_EXPORT mafNetworkConnectorREST : public mafNetworkConnector {
    Q_OBJECT

Q_SIGNALS:

public Q_SLOTS:
    /// Allow to send a network request.
    /** Contains the conversion between maf datatypes and qxmlrpc datatype based both on QVariant. */
    /*virtual*/ void send(const QString event_id, mafEventArgumentsList *argList, bool externalSend = false);

private Q_SLOTS:

public:
    /// object constructor.
    mafNetworkConnectorREST();

    /// object destructor.
    /*virtual*/ ~mafNetworkConnectorREST();

    /// create the unique instance of the client.
    /*virtual*/ void createClient(const QString hostName, const unsigned int port);

    /// create the unique instance of the server.
    /*virtual*/ void createServer(const unsigned int port);

    /// Start the server.
    /*virtual*/ void startListen();

    //retrieve an instance of the object
    /*virtual*/ mafNetworkConnector *clone();

    /// register all the signals and slots
    /*virtual*/ void initializeForEventBus();


protected:

private:
};

} //namespace mafEventBus



#endif // MAFNETWORKCONNECTORREST_H
