/*
 *  mafEventDispatcherRemote.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENTDISPATCHERREMOTE_H
#define MAFEVENTDISPATCHERREMOTE_H

//include list
#include "mafEventDefinitions.h"
#include "mafEventDispatcher.h"

namespace mafEventBus {

class mafNetworkConnector;

/**
 Class name: mafEventDispatcherRemote
 This allows dispatching events coming from local application to attached observers.
 */
class MAFEVENTBUSSHARED_EXPORT mafEventDispatcherRemote : public mafEventDispatcher {
    Q_OBJECT

public:
    /// object constructor.
    mafEventDispatcherRemote();

    /// object destructor.
    /*virtual*/ ~mafEventDispatcherRemote();

    /// Emit event corresponding to the given id (present into the event_dictionary) locally to the application.
    /*virtual*/ void notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList = NULL, QGenericReturnArgument *returnArg = NULL) const;

    /// Set the active server side network connector type.
    void setNetworkConnectorServer(mafNetworkConnector *connector);

    /// return the current instantiated server side network connector.
    mafNetworkConnector *networkConnectorServer();

    /// Set the active client side network connector type.
    void setNetworkConnectorClient(mafNetworkConnector *connector);

    /// return the current instantiated client side network connector.
    mafNetworkConnector *networkConnectorClient();

protected:
    /// Register MAF global events
    /*virtual*/ void initializeGlobalEvents();

private:
    mafNetworkConnector *m_NetworkConnectorServer; ///< Class used to manage the communication with the remote event bus.
    mafNetworkConnector *m_NetworkConnectorClient; ///< Class used to manage the communication with the remote event bus.
};


} //namespace mafEventBus

Q_DECLARE_METATYPE(QList<QVariant>);

#endif // MAFEVENTDISPATCHERREMOTE_H
