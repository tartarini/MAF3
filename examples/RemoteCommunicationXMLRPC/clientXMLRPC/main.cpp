/*
 *  main.cpp
 *  clientXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <QtCore/QCoreApplication>
#include <mafCoreSingletons.h>
#include "mafClientXMLRPC.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QByteArray ba;
    if(argc != 3) {
        ba = mafTr("Usage: \nclientXMLRPC port server_address").toAscii();
        qDebug("%s", ba.data());
        exit(0);
    }

    mafClientXMLRPC client;
    QString port(argv[1]);
    QString address(argv[2]);
    client.connectToServer(port.toUInt(), address);

    //event dictionary
    mafEventBus::mafEvent event_dictionary;
    event_dictionary[TOPIC] = "maf.remote.eventBus.communication.send.xmlrpc";
    event_dictionary[TYPE] = mafEventBus::mafEventTypeRemote;

    //remote event parameters
    mafEventBus::mafEventArgumentsList listToSend;
    QVariantList eventParameters;
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    QVariantList dataParameters;
    dataParameters.append("mafOperationLogger");

    QVariantMap operationParameters;
    operationParameters.insert("dataBaseName", "StiCazziDiDB.db");
    operationParameters.insert("tableName", "logTable");
    operationParameters.insert("workflowID", "GUQFEFQUYWJGQJWQBHWGJHWBQHBW");
    dataParameters.push_back(operationParameters);

    listToSend.append(Q_ARG(QVariantList, dataParameters));


    client.sendRequestToServer(&event_dictionary, &listToSend);

    return a.exec();
}
