/*
 *  mafEventDefinitions.h
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
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
#define mafSoapMessage QtSoapMessage
#define mafSoapHttpTransport QtSoapHttpTransport
#define mafSoapType QtSoapType
#define mafSoapSimpleType QtSoapSimpleType
#define mafSoapArray QtSoapArray
#define mafSoapName QtSoapName
#define mafSoapQName QtSoapQName

#define mafString QString
#define mafChar QChar
#define mafByteArray QByteArray
#define mafStringList QStringList
#define mafVariant QVariant
#define mafVariantList QVariantList

#define mafList QList
#define mafLinkedList QLinkedList
#define mafLinkedListIterator QLinkedListIterator
#define mafHash QHash
#define mafMap QMap
#define mafTr QObject::tr

#define mafMsgDebug qDebug
#define mafMsgFatal qFatal
#define mafMsgCritical qCritical
#define mafMsgWarning qWarning


#define TOPIC "EventTopic"
#define TYPE "EventType"
#define SIGTYPE "SignatureType"
#define OBJECT "ObjectPointer"
#define SIGNATURE "Signature"

#define mafRegisterLocalSignal(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeSignal, static_cast<QObject*>(sender), signature); \
        bool ok = mafEventBus::mafTopicRegistry::instance()->registerTopic(topic, sender);\
        if(ok) {\
            ok = mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
             }\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the signal registration with ID '%1'.").arg(topic).toAscii().data());\
            if(properties) {delete properties; properties = NULL;} \
            }\
    }

#define mafRegisterRemoteSignal(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeRemote, mafEventBus::mafSignatureTypeSignal, static_cast<QObject*>(sender), signature); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the signal registration with ID '%1'.").arg(topic).toAscii().data());\
            if(properties) {delete properties; properties = NULL;} \
        }\
    }

#define mafRegisterLocalCallback(topic, observer, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeCallback, static_cast<QObject*>(observer), signature); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the callback registration with ID '%1'.").arg(topic).toAscii().data());\
            if(properties) {delete properties; properties = NULL;} \
        }\
    }

#define mafRegisterRemoteCallback(topic, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent(topic, mafEventBus::mafEventTypeRemote, mafEventBus::mafSignatureTypeCallback, static_cast<QObject*>(sender), signature); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the callback registration with ID '%1'.").arg(topic).toAscii().data());\
            if(properties) {delete properties; properties = NULL;} \
        }\
    }


namespace mafEventBus {
//forward classes
class mafNetworkConnector;

/// Hash table that associate the communication protocol with the corresponding network connector class (Eg. XMLRPC, mafEventBus::mafNetworkConnectorQXMLRPC)
typedef QHash<mafString, mafNetworkConnector *> mafNetworkConnectorHash;

/// typedef that represents dictionary entries ( key  , value )
typedef QHash<mafString, mafVariant> mafEventHash;

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
#define mafGenericReturnArgument QGenericReturnArgument
#define mafEventReturnArgument(type, data) QReturnArgument<type >(#type, data)

class mafEvent;

/// Types definitions for events' hash (to be more readable).
typedef QHash<mafString, mafEvent *> mafEventsHashType;

/// type definition for observers' properties list to be stored into the event's hash.
typedef QList<mafEvent *> mafEventItemListType;

/// map which represent list of function to be registered in the server, with parameters
typedef QMap<mafString, mafList<mafVariant::Type> >  mafRegisterMethodsMap;

} // namespace mafEventBus


Q_DECLARE_METATYPE(mafEventBus::mafEventArgumentsListPointer);
Q_DECLARE_METATYPE(mafEventBus::mafRegisterMethodsMap);

#endif // MAFEVENTDEFINITIONS_H
