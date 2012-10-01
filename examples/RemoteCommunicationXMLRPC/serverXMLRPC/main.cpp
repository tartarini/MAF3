/*
 *  main.cpp
 *  serverXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <QtCore/QCoreApplication>
#include <mafCoreSingletons.h>
#include "mafServerXMLRPC.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        qDebug() << mafTr("Usage: \nserverXMLRPC port");
        exit(0);
    }

    QCoreApplication a(argc, argv);

    mafServerXMLRPC server;
    QString port(argv[1]);
    server.startListen(port.toUInt());

    return a.exec();
}
