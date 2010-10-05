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
    if(m_Http) delete m_Http;
}

mafNetworkConnector *mafNetworkConnectorQtSoap::clone() {
    mafNetworkConnectorQtSoap *copy = new mafNetworkConnectorQtSoap();
    return copy;
}

void mafNetworkConnectorQtSoap::registerServerMethod(mafString methodName, mafList<mafVariant::Type> types) {
   m_RegisterMethodsMap.insert(methodName, types);
}

void mafNetworkConnectorQtSoap::createClient(const mafString hostName, const unsigned int port) {
    if(m_Http == NULL) {
        m_Http = new mafSoapHttpTransport();
        QObject::connect(m_Http, SIGNAL(responseReady()), this, SLOT(retrieveRemoteResponse()));
    }

    //maf3 service registration
    mafList<mafVariant::Type> parametersForRegisterteredFunction;
    parametersForRegisterteredFunction.append(mafVariant::String); //return argument
    parametersForRegisterteredFunction.append(mafVariant::List); //parameters to send, event control parameters
    parametersForRegisterteredFunction.append(mafVariant::List); //parameters to send, data parameters

    //registration of the method REMOTE_COMMUNICATION_SOAP at Soap level
    // this method need to reflect the name of the action of the service while mafVariant::List are list of
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
    
    mafMsgDebug() << request.toXmlString();

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
    mafMsgDebug() << mafTr("QtSoap doesn't support server side implementation.").toAscii();
}

void mafNetworkConnectorQtSoap::stopServer() {
    mafMsgDebug() << mafTr("QtSoap doesn't support server side implementation.").toAscii();
}

void mafNetworkConnectorQtSoap::startListen() {
    mafMsgDebug() << mafTr("QtSoap doesn't support server side implementation.").toAscii();
}

mafSoapType *mafNetworkConnectorQtSoap::marshall(const mafString name, const mafVariant &parameter) {
    mafSoapType *returnValue = NULL;
    switch( parameter.type() ){
        case mafVariant::Int:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), mafString::number(parameter.toInt()));
                break;
        case mafVariant::UInt:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), mafString::number(parameter.toUInt()));
                break;
        case mafVariant::LongLong:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), mafString::number(parameter.toLongLong()));
                break;
        case mafVariant::ULongLong:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), mafString::number(parameter.toULongLong()));
                break;
        case mafVariant::Double:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), mafString::number(parameter.toDouble()));
                break;
        case mafVariant::Bool:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), parameter.toBool()?"True":"False");
                break;
        case mafVariant::Date:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), parameter.toDate().toString());
                break;
        case mafVariant::DateTime:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), parameter.toDateTime().toString());
                break;
        case mafVariant::Time:
                returnValue = new mafSoapSimpleType(mafSoapQName(name), parameter.toTime().toString());
                break;
        case mafVariant::StringList:
        case mafVariant::List: {
                mafSoapArray *arr = new mafSoapArray(mafSoapQName(name, ""), mafSoapType::String, parameter.toList().size());
                int index = 0;
                foreach( mafVariant item, parameter.toList() ) {
                    arr->insert(index, marshall(mafString("Elem_").append(mafString::number(index)), item ));
                    index++;
                    }
                returnValue = arr;
                break;
        }
        case mafVariant::Map: {
            mafMap<mafString, mafVariant> map = parameter.toMap();
            mafMap<mafString, mafVariant>::ConstIterator iter = map.begin();
            mafSoapArray *arr = new mafSoapArray(mafSoapQName(name, ""), mafSoapType::String, parameter.toMap().size());
            int index = 0;
            while( iter != map.end() ) {
                arr->insert(index, marshall(iter.key(), *iter));
                ++iter;
                index++;
            }
            returnValue = arr;
            break;
        }
        case mafVariant::Hash: {
            mafHash<mafString, mafVariant> hash = parameter.toHash();
            mafHash<mafString, mafVariant>::ConstIterator iter = hash.begin();
            mafSoapArray *arr = new mafSoapArray(mafSoapQName(name, ""), mafSoapType::String, parameter.toHash().size());
            int index = 0;
            while( iter != hash.end() ) {
                arr->insert(index, marshall(iter.key(), *iter));
                ++iter;
                index++;
            }
            returnValue = arr;
            break;
        }
        case mafVariant::ByteArray: {
            returnValue = new mafSoapSimpleType(mafSoapQName(name), parameter.toByteArray().data());
            break;
        }
        default: {
            if( parameter.canConvert(mafVariant::String) ) {
                returnValue = new mafSoapSimpleType(mafSoapQName(name), parameter.toString());
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

void mafNetworkConnectorQtSoap::send(const mafString &methodName, mafList<mafVariant> *params) {
    //REQUIRE(!params->at(0).isNull());
    //REQUIRE(params->at(0).canConvert(mafVariant::Hash) == true);

    m_Request.clear();
    m_Request.setMethod(methodName);
    mafHash<mafString, mafVariant> *values;
    values = &(params->at(0).toHash());
    int i = 0, size = values->size();
    for(;i<size;i++) {
        m_Request.addMethodArgument(marshall(values->keys().at(i), values->values().at(i)));
    }

    mafMsgDebug() << m_Request.toXmlString();

    // Submit the request the the web service.
    m_Http->setAction(m_Action);
    m_Http->submitRequest(m_Request, m_Path);


}

void mafNetworkConnectorQtSoap::retrieveRemoteResponse()
{
    // Get a reference to the response message.
    const mafSoapMessage &message = m_Http->getResponse();
    mafMsgDebug() << message.toXmlString();
    // Check if the response is a SOAP Fault message
    if (message.isFault()) {
        mafMsgDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
        m_Response = NULL;
    }
    else {
        // Get the return value, and print the result.
        m_Response = const_cast<mafSoapType *>( &(message.returnValue()));
    }
}


/*
void mafNetworkConnectorQtSoap::processReturnValue( int requestId, QVariant value ) {
    Q_UNUSED( requestId );
    Q_ASSERT( value.canConvert( QVariant::String ) );
    mafMsgDebug("%s", value.toString().toAscii().data());
    mafEventBusManager::instance()->notifyEvent("REMOTE_COMMUNICATION_DONE", mafEventTypeLocal);
}

void mafNetworkConnectorQtSoap::processFault( int requestId, int errorCode, QString errorString ) {
    // Log the error.
    mafMsgDebug("%s", mafTr("Process Fault for requestID %1 with error %2 - %3").arg(mafString::number(requestId), mafString::number(errorCode), errorString).toAscii().data());
    mafEventBusManager::instance()->notifyEvent("REMOTE_COMMUNICATION_FAILED", mafEventTypeLocal);
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
        mafCore::mafId id = mafCore::mafIdProvider::instance()->idValue(id_name);
        dictionary.setEventId(id);
        dictionary.setEventType(mafEventTypeLocal);
        mafEventBusManager::instance()->notifyEvent(dictionary, argList);
        m_Server->sendReturnValue( requestId, mafString("OK") );
    } else {
        m_Server->sendReturnValue( requestId, mafString("FAIL") );
    }
    mafDEL(argList);
}*/

