/*
 *  mafNetworkConnector.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNETWORKCONNECTOR_H
#define MAFNETWORKCONNECTOR_H

//include list
#include "mafEventDefinitions.h"

namespace mafEventBus {

/**
 Class name: mafNetworkConnector
 This class is the interface class for client/server objects that works over network.
 */
class MAFEVENTBUSSHARED_EXPORT mafNetworkConnector : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString protocol READ protocol);

public:
    /// object constructor.
    mafNetworkConnector();

    /// create the unique instance of the client.
    virtual void createClient(const QString hostName, const unsigned int port) = 0;

    /// create the unique instance of the server.
    virtual void createServer(const unsigned int port) = 0;

    /// Start the server.
    virtual void startListen() = 0;

    /// Allow to send a network request.
    virtual void send(const QString event_id, mafEventArgumentsList *argList) = 0;

    /// retrieve an instance of the object
    virtual mafNetworkConnector *clone() = 0;

    /// retrieve the protocol type of the connector
    QString protocol();

    /// register all the signals and slots
    virtual void initializeForEventBus() = 0;

signals:
    /// signal for send a message to through network
    void remoteCommunication(const QString event_id, mafEventArgumentsList *argList);

protected:
    QString m_Protocol; ///< define the protocol of the connector (xmlrpc, soap, etc...)
};

} //namespace mafEventBus

#endif // MAFNETWORKCONNECTOR_H
