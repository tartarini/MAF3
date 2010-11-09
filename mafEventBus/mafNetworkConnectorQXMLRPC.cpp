/*
 *  mafNetworkConnectorQXMLRPC.cpp
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNetworkConnectorQXMLRPC.h"
#include "mafEventBusManager.h"

#include "mafEvent.h"

using namespace mafEventBus;
//using namespace mafCore;

mafNetworkConnectorQXMLRPC::mafNetworkConnectorQXMLRPC() : mafNetworkConnector(), m_Client(NULL), m_Server(NULL), m_RequestId(0) {
    //generate remote signal, this signal must map in the
    //possible connection with the remote server.
    //Server, in this case XMLRPC, will register a method with id REMOTE_COMMUNICATION
    //and parameters mafList<mafVariant>

    m_Protocol = "XMLRPC";

    mafRegisterRemoteSignal("maf.remote.eventBus.comunication.xmlrpc", NULL, "remoteCommunication()");
}

mafNetworkConnectorQXMLRPC::~mafNetworkConnectorQXMLRPC() {
    if(m_Client) {
        delete m_Client;
        m_Client = NULL;
    }
    if(m_Server) {
        stopServer();
    }
}

//retrieve an instance of the object
mafNetworkConnector *mafNetworkConnectorQXMLRPC::clone() {
    mafNetworkConnectorQXMLRPC *copy = new mafNetworkConnectorQXMLRPC();
    return copy;
}

void mafNetworkConnectorQXMLRPC::createClient(const mafString hostName, const unsigned int port) {
    bool result(false);
    if(m_Client == NULL) {
        m_Client = new xmlrpc::Client(NULL);
        result = connect( m_Client, SIGNAL(done( int, QVariant )),
                 this, SLOT(processReturnValue( int, QVariant )) );
        result = connect( m_Client, SIGNAL(failed( int, int, QString )),
                 this, SLOT(processFault( int, int, QString )) );
    }
    m_Client->setHost( hostName, port );
}

void mafNetworkConnectorQXMLRPC::createServer(const unsigned int port) {
    if(m_Server != NULL) {
        unsigned int p = m_Server->property("port").toUInt();
        if(p != port) {
            stopServer();
        }
    }
    m_Server = new xmlrpc::Server(NULL);
    m_Server->setProperty("port", port);

    // Create a new ID to allow methods registration on new server instance.
    mafString id_name(mafTr("REGISTER_SERVER_METHODS_%1").arg(port));
    //mafId register_methods_id = mafIdProvider::instance()->createNewId(id_name);

    // Register the signal to the event bus.
    mafRegisterLocalSignal(id_name, this, "registerMethodsServer(mafRegisterMethodsMap)");
    connect(this, SIGNAL(registerMethodsServer(mafRegisterMethodsMap)),
            this, SLOT(registerServerMethod(mafRegisterMethodsMap)));

    mafList<mafVariant::Type> parametersForRegisterteredFunction;
    parametersForRegisterteredFunction.append(mafVariant::String); //return argument
    parametersForRegisterteredFunction.append(mafVariant::List); //parameters to send, event control parameters
    parametersForRegisterteredFunction.append(mafVariant::List); //parameters to send, data parameters

    //registration of the method maf.remote.eventBus.comunication.xmlrpc at XMLRPC level
    // the connect uses function name ad signature defined by parametersForRegisterteredFunction
    mafRegisterMethodsMap methodsMapping;
    methodsMapping.insert("maf.remote.eventBus.comunication.xmlrpc", parametersForRegisterteredFunction);
    registerServerMethod(methodsMapping);

    //if a user want to register another method, it is important to know that mafEventDispatcherRemote allows
    // the registration of function with mafList<mafVariant> parameter.
}

void mafNetworkConnectorQXMLRPC::stopServer() {
    unsigned int p = m_Server->property("port").toUInt();
    if(p != 0) {
        // get the ID for the previous server;
        mafString old_id_name(mafTr("REGISTER_SERVER_METHODS_%1").arg(p));
        //mafId old_register_methods_id = mafIdProvider::instance()->createNewId(old_id_name);
        // Remove the old signal.
        mafEvent props;
        props[TOPIC] = old_id_name;
        props[TYPE] = mafEventTypeLocal;
        props[SIGNATURE] = "registerMethodsServer(mafRegisterMethodsMap)";
        mafEventBusManager::instance()->removeEventProperty(props);
        // Delete (and stop) the previous instance of the server.
        if(m_Server) {
            delete m_Server;
            m_Server = NULL;
        }
    }
}

void mafNetworkConnectorQXMLRPC::registerServerMethod(mafRegisterMethodsMap registerMethodsList) {
    if(m_Server->isListening()) {
        mafMsgDebug("%s", mafTr("Server is already listening on port %1").arg(m_Server->property("port").toUInt()).toAscii().data());
        return;
    }
    // cycle over map:  method name and parameter list
    foreach (mafString key, registerMethodsList.keys()) {
       const unsigned int parametersNumber = registerMethodsList.value(key).count();
       switch(parametersNumber) {
       case 1:
           m_Server->registerMethod(key, registerMethodsList.value(key).at(0) );
           break;
       case 2:
           m_Server->registerMethod(key, registerMethodsList.value(key).at(0), registerMethodsList.value(key).at(1));
           break;
       case 3:
           m_Server->registerMethod(key, registerMethodsList.value(key).at(0), registerMethodsList.value(key).at(1), registerMethodsList.value(key).at(2));
           break;
       case 4:
           m_Server->registerMethod(key, registerMethodsList.value(key).at(0), registerMethodsList.value(key).at(1), registerMethodsList.value(key).at(2), registerMethodsList.value(key).at(3));
           break;
       case 5:
           m_Server->registerMethod(key, registerMethodsList.value(key).at(0), registerMethodsList.value(key).at(1), registerMethodsList.value(key).at(2), registerMethodsList.value(key).at(3) , registerMethodsList.value(key).at(4));
           break;
       default:
           break;
       }
    }
}

void mafNetworkConnectorQXMLRPC::startListen() {
    connect( m_Server, SIGNAL(incomingRequest( int, QString, QList<xmlrpc::Variant>)),
                 this, SLOT(processRequest( int, QString, QList<xmlrpc::Variant>)));

    unsigned int port = m_Server->property("port").toInt();
    if( m_Server->listen( port ) ) {
        qDebug() << "Listening for XML-RPC requests on port" << port;
    } else {
        qDebug() << "Error listening port" << port;
    }
}

void mafNetworkConnectorQXMLRPC::send(const mafString &event_id, mafList<mafVariant> *params) {
   //conversion between MAF datatypes and qxmlrpc datatypes based on QVariant
   mafList<xmlrpc::Variant> convertedParams;

   int i=0, size = params->count();
   for(;i<size;i++) {
       xmlrpc::Variant varQ;
       varQ.setValue(params->at(i));
       convertedParams.push_back(varQ);
   }

   xmlrpcSend(event_id, convertedParams);
}

void mafNetworkConnectorQXMLRPC::xmlrpcSend(const mafString &methodName, mafList<xmlrpc::Variant> parameters) {
    const unsigned int parametersNumber = parameters.count();
    switch(parametersNumber) {
    case 1:
        m_RequestId = m_Client->request(methodName, parameters.at(0) );
        break;
    case 2:
        m_RequestId = m_Client->request(methodName, parameters.at(0), parameters.at(1));
        break;
    case 3:
        m_RequestId = m_Client->request(methodName, parameters.at(0), parameters.at(1), parameters.at(2));
        break;
    case 4:
        m_RequestId = m_Client->request(methodName, parameters.at(0), parameters.at(1), parameters.at(2), parameters.at(3));
        break;
    default:
        break;
    }
}

void mafNetworkConnectorQXMLRPC::processReturnValue( int requestId, QVariant value ) {
    Q_UNUSED( requestId );
    Q_ASSERT( value.canConvert( QVariant::String ) );
    mafMsgDebug("%s", value.toString().toAscii().data());
    mafEventBusManager::instance()->notifyEvent("maf.remote.eventBus.comunicationDone", mafEventTypeLocal);
}

void mafNetworkConnectorQXMLRPC::processFault( int requestId, int errorCode, QString errorString ) {
    // Log the error.
    mafMsgDebug("%s", mafTr("Process Fault for requestID %1 with error %2 - %3").arg(mafString::number(requestId), mafString::number(errorCode), errorString).toAscii().data());
    mafEventBusManager::instance()->notifyEvent("maf.remote.eventBus.comunicationFailed", mafEventTypeLocal);
}

void mafNetworkConnectorQXMLRPC::processRequest( int requestId, QString methodName, QList<xmlrpc::Variant> parameters ) {
    Q_UNUSED( methodName );
    //REQUIRE(parameters.count() >= 2);
    //first parameter is mafEventBus message
    enum {
      EVENT_PARAMETERS,
      DATA_PARAMETERS,
    };

    enum {
      EVENT_ID,
      EVENT_ITEM_TYPE,
      EVENT_SIGNATURE_TYPE,
      EVENT_METHOD_SIGNATURE,
    };

    if(parameters.at(EVENT_PARAMETERS).toList().count() == 0) {
        m_Server->sendReturnValue( requestId, mafString("No Command to Execute, command list is empty") );
    }

    //here eventually can be used a filter for events


    //first argument regards local signal to be called.
    mafString id_name = parameters.at(EVENT_PARAMETERS).toList().at(EVENT_ID).toString();

    int size = parameters.count();

    mafEventArgumentsList *argList = NULL;
    mafList<mafVariant> *p = & (parameters.at(1).value< mafList<mafVariant> >());
    if(size > 1 && p->count() != 0) {
        argList = new mafEventArgumentsList();
        argList->push_back(Q_ARG(mafList<mafVariant>, *p));
    }

    if ( mafEventBusManager::instance()->isSignalPresent(id_name) ) {
        mafEvent dictionary;
        //mafCore::mafId id = mafCore::mafIdProvider::instance()->idValue(id_name);
        dictionary.setEventTopic(id_name);
        dictionary.setEventType(mafEventTypeLocal);
        mafEventBusManager::instance()->notifyEvent(dictionary, argList);
        m_Server->sendReturnValue( requestId, mafString("OK") );
    } else {
        m_Server->sendReturnValue( requestId, mafString("FAIL") );
    }
    if(argList){
        delete argList;
        argList = NULL;
    }
}
