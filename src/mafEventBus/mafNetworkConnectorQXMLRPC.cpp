/*
 *  mafNetworkConnectorQXMLRPC.cpp
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNetworkConnectorQXMLRPC.h"
#include "mafEventBusManager.h"
#include <iostream>
#include "mafEvent.h"

using namespace mafEventBus;

mafNetworkConnectorQXMLRPC::mafNetworkConnectorQXMLRPC() : mafNetworkConnector(), m_Client(NULL), m_Server(NULL), m_RequestId(0) {
    //generate remote signal, this signal must map in the
    //possible connection with the remote server.
    //Server, in this case XMLRPC, will register a method with id REMOTE_COMMUNICATION
    //and parameters QVariantList

    m_Protocol = "XMLRPC";
}

void mafNetworkConnectorQXMLRPC::initializeForEventBus() {
    mafRegisterRemoteSignal("maf.remote.eventBus.communication.send.xmlrpc", this, "remoteCommunication(const QString, mafEventArgumentsList *, bool)");
    mafRegisterRemoteCallback("maf.remote.eventBus.communication.send.xmlrpc", this, "send(const QString, mafEventArgumentsList *, bool )");
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

mafNetworkConnector *mafNetworkConnectorQXMLRPC::clone() {
    mafNetworkConnectorQXMLRPC *copy = new mafNetworkConnectorQXMLRPC();
    return copy;
}

void mafNetworkConnectorQXMLRPC::createClient(const QString hostName, const unsigned int port, QMap<QString,QVariant> *advancedParameters, QString path ) {
    bool result(false);
    if(m_Client == NULL) {
        m_Client = new xmlrpc::Client(NULL);
        result = connect( m_Client, SIGNAL(done( int, QVariant )),
                 this, SLOT(processReturnValue( int, QVariant )) );
        result = connect( m_Client, SIGNAL(failed( int, int, QString )),
                 this, SLOT(processFault( int, int, QString )) );
    }
    if (!m_ProxyHostName.isEmpty()) {
        if (!m_ProxyUserName.isEmpty()) {
            m_Client->setProxy(m_ProxyHostName, m_ProxyPort, m_ProxyUserName, m_ProxyPassword);
        } else {
            m_Client->setProxy(m_ProxyHostName, m_ProxyPort);
        }
    }

    QHttp::ConnectionMode cm = QHttp::ConnectionModeHttp;
    if(advancedParameters && advancedParameters->contains("connectionmode")) {
        cm = (QHttp::ConnectionMode) advancedParameters->value("connectionmode").toInt();
    }
    
    if(advancedParameters && advancedParameters->contains("username") && advancedParameters->contains("password")) {
        m_Client->setUser(advancedParameters->value("username").toString(), advancedParameters->value("password").toString());        
    }
    
    m_Client->setHost( hostName, cm, port , path);

	// set advanced paramters
	if ( advancedParameters && advancedParameters->count() ){
		setAdvancedParameters( advancedParameters );
	}

}

void mafNetworkConnectorQXMLRPC::setProxy(const QString & host, int port, const QString & userName, const QString & password) {
    m_ProxyHostName = host;
    m_ProxyPort = port;
    m_ProxyUserName = userName;
    m_ProxyPassword = password;
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
    //QString id_name(mafTr("maf.remote.eventbus.communication.send.xmlrpc.serverMethods%1").arg(port));

    // Register the signal to the event bus.
    /*mafRegisterRemoteSignal(id_name, this, "registerMethodsServer(mafRegisterMethodsMap)");
    connect(this, SIGNAL(registerMethodsServer(mafRegisterMethodsMap)),
            this, SLOT(registerServerMethod(mafRegisterMethodsMap)));*/

    QList<QVariant::Type> parametersForRegisterteredFunction;
    parametersForRegisterteredFunction.append(QVariant::Map); //return argument
    parametersForRegisterteredFunction.append(QVariant::List); //parameters to send, event control parameters
    parametersForRegisterteredFunction.append(QVariant::List); //parameters to send, data parameters

    //registration of the method maf.remote.eventBus.comunication.xmlrpc at XMLRPC level
    // the connect uses function name ad signature defined by parametersForRegisterteredFunction
    mafRegisterMethodsMap methodsMapping;
    methodsMapping.insert("maf.remote.eventBus.communication.send.xmlrpc", parametersForRegisterteredFunction);
    registerServerMethod(methodsMapping);

    //if a user want to register another method, it is important to know that mafEventDispatcherRemote allows
    // the registration of function with QVariantList parameter.
}

void mafNetworkConnectorQXMLRPC::stopServer() {
    unsigned int p = m_Server->property("port").toUInt();
    if(p != 0) {
        // get the ID for the previous server;
        /*QString old_id_name(mafTr("maf.remote.eventbus.communication.send.xmlrpc.serverMethods%1").arg(p));
        // Remove the old signal.
        mafEvent props;
        props[TOPIC] = old_id_name;
        props[TYPE] = mafEventTypeRemote;
        props[SIGNATURE] = "registerMethodsServer(mafRegisterMethodsMap)";
        mafEventBusManager::instance()->removeEventProperty(props);*/
        // Delete (and stop) the previous instance of the server.
        if(m_Server) {
            delete m_Server;
            m_Server = NULL;
        }
    }
}

void mafNetworkConnectorQXMLRPC::registerServerMethod(mafRegisterMethodsMap registerMethodsList) {
    if(m_Server->isListening()) {
        QByteArray ba = mafTr("Server is already listening on port %1").arg(m_Server->property("port").toUInt()).toAscii();
        qDebug("%s", ba.data());
        return;
    }
    // cycle over map:  method name and parameter list
    Q_FOREACH (QString key, registerMethodsList.keys()) {
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

void mafNetworkConnectorQXMLRPC::send(const QString event_id, mafEventArgumentsList *argList, bool externalSend) {
    QList<xmlrpc::Variant> *vl = NULL;
    xmlrpc::Variant var;
    QByteArray ba;
    if(argList != NULL) {
        vl = new QList<xmlrpc::Variant>();

        int i=0, size = argList->count();
        for(;i<size;++i) {
            QString typeArgument;
            typeArgument = argList->at(i).name();
            if (!externalSend) {
                void *vp = argList->at(i).data();
                if(typeArgument == "QString"){
                    QString v;
                    v = *((QString*)vp);
                    var.setValue(v);
                } else if(typeArgument == "double"){
                    double *v = (double*)vp;
                    var.setValue(*v);
                } else if (typeArgument == "int") {
                    int *v = (int*)vp;
                    var.setValue(*v);
                } else if (typeArgument == "QVariantMap") {
                    QVariantMap *v;
                    v = (QVariantMap *)vp;
                    var.setValue(*v);
                } else if (typeArgument == "QVariantList") {
                    QVariantList *v;
                    v = (QVariantList *)vp;
                    var.setValue(*v);
                } else {
                    ba = mafTr("Remote Dispatcher needs to have compatible arguments ").toAscii();
                    qWarning("%s", ba.data());
                    delete vl;
                    return;
                }                
            } else {
                void *vp = argList->at(i).data();
                if(typeArgument == "QString"){
                    QString v;
                    v = *((QString*)vp);
                    var.setValue(v);
                } else if(typeArgument == "double"){
                    double *v = (double*)vp;
                    var.setValue(*v);
                } else if (typeArgument == "int") {
                    int *v = (int*)vp;
                    var.setValue(*v);
                } else if (typeArgument == "QVariantMap") {
                    QVariantMap *v;
                    v = (QVariantMap *)vp;
                    var.setValue(*v);
                } else if (typeArgument == "QVariantList") {
                    QVariantList *v;
                    v = (QVariantList *)vp;
                    var.setValue(*v);
                }
            }
            vl->push_back(var); //only the first parameter represent the whole list of arguments
        }
        if(size == 0) {
            ba = mafTr("Remote Dispatcher need to have at least one argument that is a QVariantList").toAscii();
            qWarning("%s", ba.data());
            return;
        }
    }

    if(vl == NULL && externalSend) {
        QList<xmlrpc::Variant> params;
        xmlrpcSend(event_id, params);
        return;
    }

    if(vl == NULL) {
        qWarning() << "An EventBus message needs a list as argument";
        return;
    }

    xmlrpcSend(event_id, *vl);
    delete vl;

}

void mafNetworkConnectorQXMLRPC::xmlrpcSend(const QString &methodName, QList<xmlrpc::Variant> parameters) {
    m_Client->request(parameters, methodName, m_AdvancedParameters); 
}

void mafNetworkConnectorQXMLRPC::processReturnValue( int requestId, QVariant value ) {
    Q_UNUSED( requestId );
    
    Q_EMIT( updatedResponseSignal( value ));
    
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QVariant, value));
    mafEventBusManager::instance()->notifyEvent("maf.local.eventBus.remoteCommunicationDone", mafEventTypeLocal, &argList);
    
}

void mafNetworkConnectorQXMLRPC::processFault( int requestId, int errorCode, QString errorString ) {
    // Log the error.
    QByteArray ba = mafTr("Process Fault for requestID %1 with error %2 - %3").arg(QString::number(requestId), QString::number(errorCode), errorString).toAscii();
    qDebug("%s", ba.data());
    QVariant value;
    QVariantMap errorMap;
    errorMap.insert("errorCode", (QVariant)errorCode);
    errorMap.insert("errorString", (QVariant)errorString);

    value.setValue(errorMap);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QVariant, value));
    mafEventBusManager::instance()->notifyEvent("maf.local.eventBus.remoteCommunicationFailed", mafEventTypeLocal, &argList);
}

void mafNetworkConnectorQXMLRPC::processRequest( int requestId, QString methodName, QList<xmlrpc::Variant> parameters ) {
    Q_UNUSED( methodName );

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

    QMap<QString, xmlrpc::Variant> returnMap;
    if(parameters.at(EVENT_PARAMETERS).toList().count() == 0) {
        returnMap.insert("returnValue", "FAIL");
        returnMap.insert("errorMessage", mafTr("No Command to Execute, command list is empty"));
        m_Server->sendReturnValue( requestId, returnMap );
        return;
    }

    //here eventually can be used a filter for events

    //first argument regards local signal to be called.
    QString id_name = parameters.at(EVENT_PARAMETERS).toList().at(EVENT_ID).toString();

    int size = parameters.count();

    mafEventArgumentsList *argList = NULL;
    QVariantList p;
    p.append((parameters.at(1).value< QVariantList >()));
    if(size > 1 && p.count() != 0) {
        argList = new mafEventArgumentsList();
        argList->push_back(Q_ARG(QVariantList, p));
    }

    if ( mafEventBusManager::instance()->isLocalSignalPresent(id_name) ) {
        mafEvent dictionary;
        dictionary.setEventTopic(id_name);
        dictionary.setEventType(mafEventTypeLocal);
        mafEventBusManager::instance()->notifyEvent(dictionary, argList);

        mafEvent getResponse;
        dictionary.setEventTopic("maf.local.eventBus.getResponseToNetwork");
        dictionary.setEventType(mafEventTypeLocal);
        QVariantMap m;
        QGenericReturnArgument ret_val = mafEventReturnArgument(QVariantMap, m);
        mafEventBusManager::instance()->notifyEvent(dictionary, NULL, &ret_val);
        
        QMapIterator<QString, QVariant> i(m);
        while (i.hasNext()) {
            i.next();
            
            if (i.value().canConvert(QVariant::StringList) || i.value().canConvert(QVariant::List)) {
                returnMap.insert(i.key(), i.value().toStringList());
            } else {
                returnMap.insert(i.key(), i.value().toString());
            }
            
            
        }
        
        //returnMap.insert("returnValue", "OK");
        
        /*QMapIterator<QString, xmlrpc::Variant> it(returnMap);
        while (it.hasNext()) {
            it.next();
            qDebug() << it.key() << ": " << it.value() << endl;
        }*/
        //unite
    } else {
        returnMap.insert("error", "method not exist");
    }
    m_Server->sendReturnValue( requestId, returnMap );

    if(argList){
        delete argList;
        argList = NULL;
    }
}
