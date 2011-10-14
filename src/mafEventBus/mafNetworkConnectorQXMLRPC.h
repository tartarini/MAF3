/*
 *  mafNetworkConnectorQXMLRPC.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNETWORKCONNECTORQXMLRPC_H
#define MAFNETWORKCONNECTORQXMLRPC_H

// include list
#include "mafNetworkConnector.h"

// Foundation Library include list
#include <client.h>
#include <server.h>

namespace mafEventBus {

/**
 Class name: mafNetworkConnectorQXMLRPC
 This class is the implementation class for client/server objects that works over network
 with xml-rpc protocol. The server side part also create a new ID named REGISTER_SERVER_METHODS_XXX
 (where the XXX is the port on which run the server) that allows you to register your own remote
 callbacks. The library used is qxmlrpc.
 */
class MAFEVENTBUSSHARED_EXPORT mafNetworkConnectorQXMLRPC : public mafNetworkConnector {
    Q_OBJECT


public:
    /// object constructor.
    mafNetworkConnectorQXMLRPC();

    /// object destructor.
    /*virtual*/ ~mafNetworkConnectorQXMLRPC();

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

Q_SIGNALS:
    /// signal for the registration of the functions with parameters
    void registerMethodsServer(mafRegisterMethodsMap registerMethodsList);

public Q_SLOTS:
    /// register methods on the server
    void registerServerMethod(mafRegisterMethodsMap registerMethodsList);

    /// Allow to send a network request.
    /** Contains the conversion between maf datatypes and qxmlrpc datatype based both on QVariant. */
    /*virtual*/ void send(const QString event_id, mafEventArgumentsList *argList, bool externalSend = false);

private Q_SLOTS:
    /// callback for the client which retrieve the variable from the server
    virtual void processReturnValue( int requestId, QVariant value );

    /// callback which manage a fault in the connection
    void processFault( int requestId, int errorCode, QString errorString );

    /// callback for the server which receive a request to be processed
    virtual void processRequest( int requestId, QString methodName, QList<xmlrpc::Variant> parameters );

protected:
    xmlrpc::Client *m_Client; ///< xml-rpc client provided by qxmlrpc library
    xmlrpc::Server *m_Server; ///< xml-rpc server provided by qxmlrpc library

private:
    /// send a request from the client to the network.
    void xmlrpcSend(const QString &methodName, QList<xmlrpc::Variant> parameters);

    /// stop and destroy the server instance.
    void stopServer();

    int m_RequestId; ///< id test for a specific (experimental) request
};

} //namespace mafEventBus

Q_DECLARE_METATYPE(xmlrpc::Variant);


#endif // MAFNETWORKCONNECTORQXMLRPC_H
