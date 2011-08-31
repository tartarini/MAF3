/*
 *  mafNetworkConnectorQtSoap.cpp
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 14/07/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNetworkConnectorQtSoap.h"
#include "mafEventBusManager.h"

#include "mafEvent.h"

using namespace mafEventBus;

mafNetworkConnectorQtSoap::mafNetworkConnectorQtSoap() : mafNetworkConnector(), m_Http(NULL), m_WSDLUrl(""),m_Response(NULL) {
    m_Protocol = "SOAP";
}

mafNetworkConnectorQtSoap::~mafNetworkConnectorQtSoap() {
    if(m_Http) {
        delete m_Http;
    }
}

mafNetworkConnector *mafNetworkConnectorQtSoap::clone() {
    mafNetworkConnectorQtSoap *copy = new mafNetworkConnectorQtSoap();
    return copy;
}

void mafNetworkConnectorQtSoap::initializeForEventBus() {
    mafRegisterRemoteSignal("maf.remote.eventBus.comunication.soap", this, "remoteCommunication(const QString, mafEventArgumentsList *)");
    mafRegisterRemoteCallback("maf.remote.eventBus.comunication.soap", this, "send(const QString, mafEventArgumentsList *)");
}


void mafNetworkConnectorQtSoap::registerServerMethod(QString methodName, QList<QVariant::Type> types) {
   m_RegisterMethodsMap.insert(methodName, types);
}

void mafNetworkConnectorQtSoap::createClient(const QString hostName, const unsigned int port) {
    if(m_Http == NULL) {
        m_Http = new QtSoapHttpTransport();
        QObject::connect(m_Http, SIGNAL(responseReady()), this, SLOT(retrieveRemoteResponse()));
    }

    //maf3 service registration
    QList<QVariant::Type> parametersForRegisterteredFunction;
    parametersForRegisterteredFunction.append(QVariant::String); //return argument
    parametersForRegisterteredFunction.append(QVariant::List); //parameters to send, event control parameters
    parametersForRegisterteredFunction.append(QVariant::List); //parameters to send, data parameters

    //registration of the method REMOTE_COMMUNICATION_SOAP at Soap level
    // this method need to reflect the name of the action of the service while QVariant::List are list of
    // strings, in  which each string represent the correct name of the parameter in the sevice function.
    registerServerMethod("testArray", parametersForRegisterteredFunction);

    //
    // Construct a method request message.


    //ARRAY TEST
    //QtSoapArray arr(QtSoapQName("arr", ""), QtSoapType::String, 2);
    //arr.insert(0, new QtSoapSimpleType(QtSoapQName("name"), "First"));
    //arr.insert(1, new QtSoapSimpleType(QtSoapQName("surName"), "Second"));

    //exp
    /*QtSoapArray arrEvent(QtSoapQName("arrEvent", ""), QtSoapType::String, 2);
    arrEvent.insert(0, new QtSoapSimpleType(QtSoapQName("n1"), "eventA1"));
    arrEvent.insert(1, new QtSoapSimpleType(QtSoapQName("n2"), "eventA2"));

    QtSoapArray arrData(QtSoapQName("arrData", ""), QtSoapType::String, 2);
    arrData.insert(0, new QtSoapSimpleType(QtSoapQName("d1"), "dataA1"));
    arrData.insert(1, new QtSoapSimpleType(QtSoapQName("d2"), "dataA2"));*/
    //end exp

    m_Http->setHost(hostName, false, port);


    //ARRAY TEST

    // Set the method and add one argument.
    /*request.setMethod("algorithmSIBA");
    request.addMethodArgument("input", "", "input.aim");
    request.addMethodArgument("output", "", "output.aim");
    
    request.addMethodArgument("gaussian", "", "0.42");
    request.addMethodArgument("load", "", "8");
    request.addMethodArgument("iteration", "", "40");
    
    qDebug() << request.toXmlString();

    // Submit the request the the web service.
    m_Http->setHost("localhost", false, 7889);
    m_Http->setAction("algorithmSIBA");*/
    //m_Http->submitRequest(m_Request, "http://localhost:7889/HelloWordService?wsdl");

    // Set the method and add one argument.
    /*request.setMethod("getPopulation", "http://www.abundanttech.com/WebServices/Population");
    request.addMethodArgument("strCountry", "", "Italy");

    // Submit the request the the web service.
    m_Http->setHost("www.abundanttech.com");
    m_Http->setAction("http://www.abundanttech.com/WebServices/Population/getPopulation");
    m_Http->submitRequest(request, "/WebServices/Population/population.asmx");*/


    qDebug("retrieve value...");
    
}

void mafNetworkConnectorQtSoap::createServer(const unsigned int port) {
    Q_UNUSED(port);
    qDebug() << mafTr("QtSoap doesn't support server side implementation.").toAscii();
}

void mafNetworkConnectorQtSoap::stopServer() {
    qDebug() << mafTr("QtSoap doesn't support server side implementation.").toAscii();
}

void mafNetworkConnectorQtSoap::startListen() {
    qDebug() << mafTr("QtSoap doesn't support server side implementation.").toAscii();
}

QtSoapType *mafNetworkConnectorQtSoap::marshall(const QString name, const QVariant &parameter) {
    QtSoapType *returnValue = NULL;
    switch( parameter.type() ){
        case QVariant::Int:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), QString::number(parameter.toInt()));
                break;
        case QVariant::UInt:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), QString::number(parameter.toUInt()));
                break;
        case QVariant::LongLong:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), QString::number(parameter.toLongLong()));
                break;
        case QVariant::ULongLong:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), QString::number(parameter.toULongLong()));
                break;
        case QVariant::Double:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), QString::number(parameter.toDouble()));
                break;
        case QVariant::Bool:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), parameter.toBool()?"True":"False");
                break;
        case QVariant::Date:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), parameter.toDate().toString());
                break;
        case QVariant::DateTime:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), parameter.toDateTime().toString());
                break;
        case QVariant::Time:
                returnValue = new QtSoapSimpleType(QtSoapQName(name), parameter.toTime().toString());
                break;
        case QVariant::StringList:
        case QVariant::List: {
                QtSoapArray *arr = new QtSoapArray(QtSoapQName(name, ""), QtSoapType::String, parameter.toList().size());
                int index = 0;
                foreach( QVariant item, parameter.toList() ) {
                    arr->insert(index, marshall(QString("Elem_").append(QString::number(index)), item ));
                    ++index;
                    }
                returnValue = arr;
                break;
        }
        case QVariant::Map: {
            QMap<QString, QVariant> map = parameter.toMap();
            QMap<QString, QVariant>::ConstIterator iter = map.begin();
            QtSoapArray *arr = new QtSoapArray(QtSoapQName(name, ""), QtSoapType::String, parameter.toMap().size());
            int index = 0;
            while( iter != map.end() ) {
                arr->insert(index, marshall(iter.key(), *iter));
                ++iter;
                ++index;
            }
            returnValue = arr;
            break;
        }
        case QVariant::Hash: {
            QVariantHash hash = parameter.toHash();
            QVariantHash::ConstIterator iter = hash.begin();
            QtSoapArray *arr = new QtSoapArray(QtSoapQName(name, ""), QtSoapType::String, parameter.toHash().size());
            int index = 0;
            while( iter != hash.end() ) {
                arr->insert(index, marshall(iter.key(), *iter));
                ++iter;
                ++index;
            }
            returnValue = arr;
            break;
        }
        case QVariant::ByteArray: {
            returnValue = new QtSoapSimpleType(QtSoapQName(name), parameter.toByteArray().data());
            break;
        }
        default: {
            if( parameter.canConvert(QVariant::String) ) {
                returnValue = new QtSoapSimpleType(QtSoapQName(name), parameter.toString());
            }
            else {
               //self representation?
            }
            break;
        }
    }

    //ENSURE(returnValue != NULL);
    return returnValue;
}

void mafNetworkConnectorQtSoap::send(const QString methodName, mafEventArgumentsList *argList) {
    //REQUIRE(!params->at(0).isNull());
    //REQUIRE(params->at(0).canConvert(QVariant::Hash) == true);

    QString type = argList->at(0).name();
    if(argList == NULL || type != "QVariantHash") {
        qDebug() << "NULL or invalid argument, nothing to send!";
        return;
    }
    m_Request.clear();
    m_Request.setMethod(methodName);
    QVariantHash *values;
    values = reinterpret_cast<QVariantHash *> (argList->at(0).data());
    int i = 0, size = values->size();
    for(;i<size;++i) {
        m_Request.addMethodArgument(marshall(values->keys().at(i), values->values().at(i)));
    }

    qDebug() << m_Request.toXmlString();

    // Submit the request the the web service.
    m_Http->setAction(m_Action);
    m_Http->submitRequest(m_Request, m_Path);

}

void mafNetworkConnectorQtSoap::retrieveRemoteResponse()
{
    // Get a reference to the response message.
    const QtSoapMessage &message = m_Http->getResponse();
    qDebug() << message.toXmlString();
    // Check if the response is a SOAP Fault message
    if (message.isFault()) {
        qDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
        m_Response = NULL;
    }
    else {
        // Get the return value, and print the result.
        m_Response = const_cast<QtSoapType *>( &(message.returnValue()));
    }
}


/*
void mafNetworkConnectorQtSoap::processReturnValue( int requestId, QVariant value ) {
    Q_UNUSED( requestId );
    Q_ASSERT( value.canConvert( QVariant::String ) );
    qDebug("%s", value.toString().toAscii().data());
    mafEventBusManager::instance()->notifyEvent("maf.local.eventBus.remoteCommunicationDone", mafEventTypeLocal);
}

void mafNetworkConnectorQtSoap::processFault( int requestId, int errorCode, QString errorString ) {
    // Log the error.
    qDebug("%s", mafTr("Process Fault for requestID %1 with error %2 - %3").arg(QString::number(requestId), QString::number(errorCode), errorString).toAscii().data());
    mafEventBusManager::instance()->notifyEvent("maf.local.eventBus.remoteCommunicationFailed", mafEventTypeLocal);
}

void mafNetworkConnectorQtSoap::processRequest( int requestId, QString methodName, QList<xmlrpc::Variant> parameters ) {
    Q_UNUSED( methodName );
    REQUIRE(parameters.count() >= 2);
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
        m_Server->sendReturnValue( requestId, QString("No Command to Execute, command list is empty") );
    }

    //here eventually can be used a filter for events


    //first argument regards local signal to be called.
    QString id_name = parameters.at(EVENT_PARAMETERS).toList().at(EVENT_ID).toString();

    int size = parameters.count();

    mafEventArgumentsList *argList = NULL;
    QVariantList *p = & (parameters.at(1).value< QVariantList >());
    if(size > 1 && p->count() != 0) {
        argList = new mafEventArgumentsList();
        argList->push_back(Q_ARG(QVariantList, *p));
    }

    if ( mafEventBusManager::instance()->isLocalSignalPresent(id_name) ) {
        mafEvent dictionary;
        mafCore::mafId id = mafCore::mafIdProvider::instance()->idValue(id_name);
        dictionary.setEventId(id);
        dictionary.setEventType(mafEventTypeLocal);
        mafEventBusManager::instance()->notifyEvent(dictionary, argList);
        m_Server->sendReturnValue( requestId, QString("OK") );
    } else {
        m_Server->sendReturnValue( requestId, QString("FAIL") );
    }
    mafDEL(argList);
}*/

