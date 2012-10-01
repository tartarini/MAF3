/*
 *  mafClientXMLRPC.h
 *  clientXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCLIENTXMLRPC_H
#define MAFCLIENTXMLRPC_H

#include <mafEventDefinitions.h>
#include <mafLogicLight.h>

class mafClientXMLRPC {
public:
    /// Object constructor.
    mafClientXMLRPC();

    /// Object destructor.
    ~mafClientXMLRPC();

    /// create connection to the server at parametric address/port
    void connectToServer(unsigned int port, QString address);

//public Q_SLOTS:
    /// send request to the server
    void sendRequestToServer(mafEventBus::mafEvent *event_dictionary, mafEventBus::mafEventArgumentsList *listToSend = NULL);

private:
    QString m_Address; /// Address giving as host or IP
    unsigned int m_Port; ///< Port on which the server will listen.
    mafApplicationLogic::mafLogicLight *m_Logic; ///< Application's logic.
};

#endif // MAFCLIENTXMLRPC_H
