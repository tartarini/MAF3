/*
 *  mafLogicLight.h
 *  mafApplicationLogic
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGICLIGHT_H
#define MAFLOGICLIGHT_H

// Includes list
#include "mafApplicationLogic_global.h"
#include <mafEventBusManager.h>
#include <mafObjectBase.h>

namespace mafApplicationLogic {

// Class forwarding list

/**
 Class name: mafLogicLight
 This class define the basic implementation of the framework logic.
 It allows simply to instantiate the event bus, register the core objects into the registry and
 on destruction allows to clean upo the registration of the objects and shutdown the event bus.
 This logic clas can be used with those applications that needs only the mafCore and mafEventBus module
 whitout the need of the mafResources module.
 */
class MAFAPPLICATIONLOGICSHARED_EXPORT mafLogicLight : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafLogicLight(const mafString code_location = "");

    /// Create and start the server for remote communication according to the given protocol and listen port.
    bool startServer(const mafString &communication_protocol, unsigned int listen_port);

    /// Create the client for remote communication according to the given protocol, server host and port.
    bool createClient(const mafString &communication_protocol, const mafString &server_host, unsigned int port);

    /// send request from Client to Server
    void sendRequestToServer(mafEventBus::mafEvent *event_dict, mafEventBus::mafEventArgumentsList *argList = NULL);

    /// Allows to initialize all the singletons and register all the objects to the factory.
    virtual bool initialize();

protected:
    /// Object destructor.
    /*virtual*/ ~mafLogicLight();

    mafEventBus::mafEventBusManager *m_EventBus; ///< Reference to the communication's event bus
};

} // namespace mafApplicationLogic

#endif // MAFLOGICLIGHT_H
