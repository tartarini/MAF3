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

//#define Operation
//#define ProgrammableOperation
#define ProgrammableOpenClinicaSearch
//#define Log

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QByteArray ba;
    if(argc != 6) {
        ba = mafTr("Usage: \nclientXMLRPC port server_address dbName workflowId serviceName").toAscii();
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
    QVariantList dataParameters;
    
#ifdef Operation    
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmInterventionalSimulation");
    
    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "GUQFEFQUYWJGQJWQBHWGJHWBQHBW");
    
    QStringList inputFiles;
    inputFiles << "L1004024_mesh.cdb" << "component_L1004024_mesh.cdb" << "4024_L1.xml";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "out.cdb" << "out.dat"; 
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef ProgrammableOperation
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmProgrammable");
//    dataParameters.append("mafAlgorithmCHA2Samp");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
//    operationParameters.insert("workflowId", "GHPHFPAHFPAHSFAHSPFAPHTP");
    operationParameters.insert("workflowId", "AGJHAGHFAHFASHFIAHSDFHIASDAHSDGI");

    QStringList inputFiles;
    inputFiles << "load_cases_CHA.csv";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
//    outputFiles << "result.txt";
    outputFiles << "load_cases.txt";
    operationParameters.insert("outputFileList", outputFiles);
    QStringList scriptFiles;
//    scriptFiles << "script.py";
    scriptFiles << "fromCHA2samp_v2.m";
    operationParameters.insert("scriptFileList", scriptFiles);
#endif

#ifdef ProgrammableOpenClinicaSearch
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    //dataParameters.append("mafAlgorithmProgrammable");
    dataParameters.append("mafAlgorithmOpenClinicaSearch");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "GHPHFPAHFPAHSFAHSPFAPHTP");

    operationParameters.insert("domain", "openclinica");
    operationParameters.insert("operation", "search");
    operationParameters.insert("requestor", "Charite");
    operationParameters.insert("patientId", "171");

    QStringList inputFiles;
    inputFiles << " ";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "resultValue.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

    
#ifdef Log    
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));
    
    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    //dataParameters.append("mafOperationManageLog");
    dataParameters.append("mafOperationLogger");
    
    QVariantMap operationParameters;
    operationParameters.insert("dataBaseName", argv[3]);
    operationParameters.insert("tableName", "logTable");
    operationParameters.insert("workflowId", argv[4]);
    operationParameters.insert("serviceName", argv[5]);
    
#endif
    
    dataParameters.push_back(operationParameters);
    listToSend.append(Q_ARG(QVariantList, dataParameters));

    client.sendRequestToServer(&event_dictionary, &listToSend);

    return a.exec();
}
