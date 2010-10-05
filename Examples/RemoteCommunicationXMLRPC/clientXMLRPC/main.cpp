/*
 *  main.cpp
 *  clientXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <QtCore/QCoreApplication>
#include <mafCoreSingletons.h>
#include "mafClientXMLRPC.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    if(argc != 3) {
        mafMsgDebug("%s", mafTr("Usage: \nclientXMLRPC port server_address").toAscii().data());
        exit(0);
    }

    mafClientXMLRPC client;
    mafString port(argv[1]);
    mafString address(argv[2]);
    client.connectToServer(port.toUInt(), address);

    //event dictionary
    mafEventBus::mafEvent event_dictionary;
    event_dictionary[TOPIC] = "REMOTE_COMMUNICATION_XMLRPC";
    event_dictionary[TYPE] = mafEventBus::mafEventTypeRemote;

    //remote event parameters
    mafEventBus::mafEventArgumentsList listToSend;
    mafList<mafVariant> eventParameters;
    eventParameters.append("RESOURCES_OPERATION_EXECUTE_WITH_PARAMETERS");
    listToSend.append(Q_ARG(mafList<mafVariant>, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of mafVariant arguments >
    mafList<mafVariant> dataParameters;
    dataParameters.append("mafAlgorithm");

    mafList<mafVariant> operationParameters;
    operationParameters.append("input");
    operationParameters.append("output");
    operationParameters.append(3);
    dataParameters.push_back(operationParameters); //here must be put a dictionary

    listToSend.append(Q_ARG(mafList<mafVariant>, dataParameters));


    client.sendRequestToServer(&event_dictionary, &listToSend);

    return a.exec();
}
