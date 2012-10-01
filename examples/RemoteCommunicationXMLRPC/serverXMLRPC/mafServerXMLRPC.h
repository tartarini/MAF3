/*
 *  mafServerXMLRPC.h
 *  serverXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERVERXMLRPC_H
#define MAFSERVERXMLRPC_H

// Includes list
#include <mafLogic.h>

// Class forwarding list
class mafAlgorithm;

/**
 Class name: mafServerXMLRPC
 This class provides the xmlrpc server that manage incoming client connections.
 */
class mafServerXMLRPC {
public:
    /// Object Constructor
    mafServerXMLRPC();

    /// Object destructor.
    ~mafServerXMLRPC();

    /// Start server listening on parametric port
    void startListen(unsigned int port);

private:
    unsigned int m_Port; ///< Port on which the server will listen.
    mafApplicationLogic::mafLogic *m_Logic; ///< Application's logic.
};

#endif // MAFSERVERXMLRPC_H
