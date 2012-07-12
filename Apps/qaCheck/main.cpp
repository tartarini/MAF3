/*
 *  main.cpp
 *  qaCheck
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 * 
 *
 */

#include <QtCore/QCoreApplication>
#include <mafCoreSingletons.h>
#include "mafQAChecker.h"

void printUsage() {
    qDebug() << mafTr("\n--------------------------------");
    qDebug() << mafTr("Usage: qaCheck command");
    qDebug() << mafTr("--------------------------------");
    qDebug() << mafTr("commands available:");
    qDebug() << mafTr("qaCheck run      python_script_to_run [parameters]");
    qDebug() << mafTr("qaCheck validate plugin_to_validate");
    qDebug() << mafTr("qaCheck profile  script_to_profile [parameters]");
    qDebug() << mafTr("\n");
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        mafCore::mafCoreSingletons::mafSingletonsInitialize();
        printUsage();
        exit(0);
    }

    QString cmd(argv[1]);
    QString param(argv[2]);

    if(cmd != "run" && cmd != "validate" && cmd != "profile") {
        printUsage();
        exit(0);
    }

    //QCoreApplication a(argc, argv);

    mafQAChecker checker;
    QStringList argList;
    if(cmd == "run") {
        int i = 3;
        for(; i < argc; ++i) {
            argList.append(argv[i]);
        }
        checker.execute(param, argList);
    } else if (cmd == "validate") {
        checker.validatePlugin(param);
    } else {
        int i = 3;
        for(; i < argc; ++i) {
            argList.append(argv[i]);
        }
        checker.executeAndProfile(param, argList);
    }

    return 0; //a.exec();
}
