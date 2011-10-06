/*
 *  mafEventDefinitions.h
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENTDEFINITIONS_H
#define MAFEVENTDEFINITIONS_H

// Qt includes
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QThread>
#include <QThreadPool>
#include <QObject>
#include <QDebug>

#include "mafEventBus_global.h"

//defines
#define mafTr QObject::tr

#define TOPIC "EventTopic"
#define TYPE "EventType"
#define SIGTYPE "SignatureType"
#define OBJECT "ObjectPointer"
#define SIGNATURE "Signature"

#define mafRegisterLocalSignal(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeSignal, static_cast<QObject*>(sender), signature); \
        bool ok = mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            QByteArray ba = mafTr("Some problem occourred during the signal registration with ID '%1'.").arg(topic).toAscii();\
            qWarning("%s", ba.data());\
            if(properties) {delete properties; properties = NULL;} \
            }\
    }

#define mafRegisterRemoteSignal(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeRemote, mafEventBus::mafSignatureTypeSignal, static_cast<QObject*>(sender), signature); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            QByteArray ba = mafTr("Some problem occourred during the signal registration with ID '%1'.").arg(topic).toAscii();\
            qWarning("%s", ba.data());\
            if(properties) {delete properties; properties = NULL;} \
        }\
    }

#define mafRegisterLocalCallback(topic, observer, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeCallback, static_cast<QObject*>(observer), signature); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            QByteArray ba = mafTr("Some problem occourred during the callback registration with ID '%1'.").arg(topic).toAscii();\
            qWarning("%s", ba.data());\
            if(properties) {delete properties; properties = NULL;} \
        }\
    }

#define mafUnregisterLocalSignal(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeSignal, static_cast<QObject*>(sender), signature); \
        mafEventBus::mafEventBusManager::instance()->removeEventProperty(*properties);\
    }

#define mafUnregisterLocalCallback(topic, observer, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeCallback, static_cast<QObject*>(observer), signature); \
        mafEventBus::mafEventBusManager::instance()->removeEventProperty(*properties);\
    }


#define mafRegisterRemoteCallback(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeRemote, mafEventBus::mafSignatureTypeCallback, static_cast<QObject*>(sender), signature); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            QByteArray ba = mafTr("Some problem occourred during the callback registration with ID '%1'.").arg(topic).toAscii();\
            qWarning("%s", ba.data());\
            if(properties) {delete properties; properties = NULL;} \
        }\
    }


namespace mafEventBus {
//forward classes
class mafNetworkConnector;

/// Hash table that associate the communication protocol with the corresponding network connector class (Eg. XMLRPC, mafEventBus::mafNetworkConnectorQXMLRPC)
typedef QHash<QString, mafNetworkConnector *> mafNetworkConnectorHash;

///< Enum that identify the mafEventType's type: Local or Remote.
typedef enum {
    mafEventTypeLocal,
    mafEventTypeRemote
} mafEventType;

///< Enum that identify the mafSignatureType's type: Signal or Callback.
typedef enum {
    mafSignatureTypeSignal = 0,
    mafSignatureTypeCallback = 1
} mafSignatureType;

/// List of the arguments to be sent through the event bus.
typedef QList<QGenericArgument> mafEventArgumentsList;
typedef mafEventArgumentsList * mafEventArgumentsListPointer;
#define mafEventArgument(type, data) QArgument<type >(#type, data)
#define mafEventReturnArgument(type, data) QReturnArgument<type >(#type, data)

class mafEvent;

/// Types definitions for events' hash (to be more readable).
typedef QHash<QString, mafEvent *> mafEventsHashType;

/// type definition for observers' properties list to be stored into the event's hash.
typedef QList<mafEvent *> mafEventItemListType;

/// map which represent list of function to be registered in the server, with parameters
typedef QMap<QString, QList<QVariant::Type> >  mafRegisterMethodsMap;

} // namespace mafEventBus


Q_DECLARE_METATYPE(mafEventBus::mafEventArgumentsListPointer);
Q_DECLARE_METATYPE(mafEventBus::mafRegisterMethodsMap);

#endif // MAFEVENTDEFINITIONS_H
