/*
 *  main.cpp
 *  clientXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <QtCore/QCoreApplication>
#include <mafCoreSingletons.h>
#include "mafClientXMLRPC.h"

//#define Operation
//#define ProgrammableOperation
//#define ProgrammableOpenClinicaSearch
//#define SimJobQsub
//#define NMSJobQSub
//#define SFJobQSub
//#define SpineJobQSub
//#define CalcRisk
//#define RiskSpine
//#define Log
//#define cha2ans
//#define NMSLoads
//#define SpineLoads
//#define FallLoads
//#define NMS2ANS
//#define Spine2ANS
//#define LoadExtr
//#define bcube
#define AnsysSearch


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

#ifdef SimJobQsub
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmSimJobQsub");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("jobsNumber", 20);
    operationParameters.insert("opProg", 0.5);
    operationParameters.insert("effTrAmd", 0);
    operationParameters.insert("workflowId", "ALEQFHPHAFSPHFAPSHFPHFPSDHF");

    QStringList inputFiles;
//    inputFiles << "load_cases_tot.txt";
    //inputFiles << "load_cases_tot.txt" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441306576" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441274629"  << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441286972" <<  "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441295344"  << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441327200"  <<  "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441332472" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-07-16.1342441338460" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702681267";
    //inputFiles << "load_cases_tot.txt" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702647587" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702612095"  << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702625587" <<  "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702636999"  << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702661463"  <<  "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702665923" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702673708" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702681267";
    inputFiles << "load_cases_tot.txt" << "Neck_Length.lis" << "CH.lis" << "Head_Radius.lis" << "keypoints_ansys.lis" << "side_femur.lis" << "Z_LT.lis" << "z_min.lis" << "femur.cdb" << "OC_extracted_data.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "Results_TOT.pdrs";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef NMSJobQSub
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmNMSJobQSub");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    //operationParameters.insert("jobsNumber", 20);
    operationParameters.insert("jobsNumber", 150);
    operationParameters.insert("opProg", 0.5);
    operationParameters.insert("effTrAmd", 0);
    operationParameters.insert("workflowId", "PEPPEHGIDGHAIGHIHKGIAJFJHJF");

    QStringList inputFiles;
//    inputFiles << "load_cases_tot.txt";
///    inputFiles << "load_cases_tot.txt" << "Neck_Length.lis" << "CH.lis" << "Head_Radius.lis" << "keypoints_ansys.lis" << "side_femur.lis" << "Z_LT.lis" << "z_min.lis" << "femur.cdb";
    inputFiles <<"load_cases.txt" << "Biom_Lenght.lis" << "CH.lis" << "Diaphysis_Lenght.lis" << "Head_Radius.lis" << "IF.lis" << "Neck_Length.lis" << "NL.lis" << "PNL.lis" << "side_femur.lis" << "Z_LT_NMS.lis" << "z_min.lis" << "LE_ME.lis" << "femur.cdb" << "MAT_ROT_ISB_GLOB.txt" << "KP_ISB.txt" << "OC_extracted_data.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "Results_NMSJob.pdrs";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef SFJobQSub
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmSFJobQSub");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    //operationParameters.insert("jobsNumber", 20);
    operationParameters.insert("jobsNumber", 4);
    operationParameters.insert("opProg", 0.5);
    operationParameters.insert("effTrAmd", 0);
    operationParameters.insert("workflowId", "MMMMMMARCOOOOOOOOOOOOAMMMMM");

    QStringList inputFiles;
//    inputFiles << "SF_load_cases.txt" <<"2844_material_mapped.cdb" << "Head_Radius.lis" << "keypoints_ansys.lis" << "Neck_Length.lis" << "side_femur.lis" << "Z_LT.lis" << "Parameters.mac" << "OC_extracted_data.txt";
    inputFiles << "SF_load_cases.txt" <<"femur.cdb" << "Head_Radius.lis" << "keypoints_ansys.lis" << "Neck_Length.lis" << "side_femur.lis" << "Z_LT.lis" << "OC_extracted_data.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "Results_TOT.pdrs";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef SpineJobQSub
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmSpineJobQSub");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("jobsNumber", 150);
    operationParameters.insert("opProg", 0.5);
    operationParameters.insert("effTrAmd", 0);
    operationParameters.insert("workflowId", "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV");

    QStringList inputFiles;

    inputFiles << "Spine_load_cases.dat" << "vertebra.cdb" << "ref_sys.txt" << "id_vertebra.info" << "OC_extracted_data.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "Results_TOT_Spine.pdrs";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef CalcRisk
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmCalculateRisk");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("yearNumber", 4);
    operationParameters.insert("workflowId", "FGHGFNMSIEOAKMVOESLLCVRT");

    QStringList inputFiles;
    //inputFiles << "Results_TOT_con_ciclo.pdrs";
    //inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-17.1347897249351";
    inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-17.1347897249351";
    //inputFiles << "https://www.physiomespace.com/Members/testUser/dataresource.2012-09-17.1347897249351";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "outLines.txt" << "RF.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef RiskSpine
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmRiskSpine");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("yearNumber", 4);
    operationParameters.insert("workflowId", "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");

    QStringList inputFiles;
    inputFiles << "esempio_di_output_simulazione_vertebrae.pdrs";
    //inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-17.1347897249351";
    //inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-17.1347897249351";
    //inputFiles << "https://www.physiomespace.com/Members/testUser/dataresource.2012-09-17.1347897249351";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "outLines.txt" << "RF.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef cha2ans
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmCHA2Ans");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("yearNumber", 4);
    operationParameters.insert("workflowId", "MAMMHGHAOIRAIXAFKDOOEJMM");

    QStringList inputFiles;
    //inputFiles << "load_cases.txt";
    inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-17.1347897249351";
    //inputFiles << "https://www.physiomespace.com/Members/testUser/dataresource.2012-09-17.1347897249351";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "load_cases_tot.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef NMSLoads
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmNMSLoads");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "OSLOFOGJSONEIFOLEPJJJJJJJJ");

    QStringList inputFiles;
    //inputFiles << "BW.txt" << "Head_Radius.lis" << "side_femur.lis" << "CH.lis" << "LE_ME.lis";
    inputFiles << "BW.txt" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702625587" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702661463" << "CH.lis" << "LE_ME.lis";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "OUTPUT_FORCES.txt" << "MAT_ROT_ISB_GLOB.txt" << "KP_ISB.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef SpineLoads
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmSpineLoads");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "OOOOOOOOOOOOOOOOOOOOOOOOOOOOO");

    QStringList inputFiles;
    inputFiles << "OC_extracted_data.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "esempio_di_output_getspineforces.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef FallLoads
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmFallLoads");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "UUUUUUUUUUUUUUUUUUUUUUUUUU");

    QStringList inputFiles;
    inputFiles << "OC_extracted_data.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "Fall_output.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef NMS2ANS
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmNMS2ANS");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "KKKKKKKKKKHGHAHHHFHHFHFHFHFHHFHFH");

    QStringList inputFiles;
    inputFiles << "OUTPUT_FORCES.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "load_cases.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef Spine2ANS
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmSpine2ANS");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("workflowId", "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");

    QStringList inputFiles;
    inputFiles << "esempio_di_output_getspineforces.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "Spine_load_cases.dat";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef LoadExtr
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmLoadsExtraction");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("myFirstParameter", 2.3);
    operationParameters.insert("mySecondParameter", "ughetto");
    operationParameters.insert("activity", "NW");
    operationParameters.insert("limit", 200);
    operationParameters.insert("workflowId", "HHHHHHHHHHHHHHHHHHHHHhhhhhh");

    QStringList inputFiles;
    //inputFiles << "ccd_angle.lis" << "Neck_Lenght.lis" << "anteversion.lis" << "weight.txt";
    inputFiles <<"https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702605495"<< "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702647587" << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702590771" << "weight.txt";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "out.txt";
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

#ifdef bcube
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmBoneMorphometry");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("evalModule", 1.0);
    operationParameters.insert("workflowId", "MAMMHGHAOIRAIXAFKDOOEJMM");

    QStringList inputFiles;
    inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-10-22.1350926551047";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "resultbcube.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif

#ifdef AnsysSearch
    eventParameters.append("maf.local.resources.operation.executeWithParameters");
    listToSend.append(Q_ARG(QVariantList, eventParameters));

    //remote data parameters (need to send for operation < nameOfOperation , List of QVariant arguments >
    dataParameters.append("mafAlgorithmAnsysTissueDBSearch");

    QVariantMap operationParameters;
    operationParameters.insert("storageServiceURI", "http://ws.physiomespace.com/WSExecute.cgi");
    operationParameters.insert("orientation", "L");
    operationParameters.insert("numberClosestBones", "2");
    operationParameters.insert("workflowId", "dannoxTestForAnsysTissueDBSearch");

    QStringList inputFiles;
    inputFiles << "https://www.physiomespace.com/Members/vphop-project/dataresource.2012-09-03.1346702681267";
    operationParameters.insert("inputFileList", inputFiles);
    QStringList outputFiles;
    outputFiles << "myoutput.txt";
    operationParameters.insert("outputFileList", outputFiles);
#endif
    
    dataParameters.push_back(operationParameters);
    listToSend.append(Q_ARG(QVariantList, dataParameters));

    client.sendRequestToServer(&event_dictionary, &listToSend);

    return a.exec();
}
