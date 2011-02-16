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
    mafMsgDebug("%s", mafTr("\n--------------------------------").toAscii().data());
    mafMsgDebug("%s", mafTr("Usage: qaCheck command").toAscii().data());
    mafMsgDebug("%s", mafTr("--------------------------------").toAscii().data());
    mafMsgDebug("%s", mafTr("commands available:").toAscii().data());
    mafMsgDebug("%s", mafTr("qaCheck run      python_script_to_run [parameters]").toAscii().data());
    mafMsgDebug("%s", mafTr("qaCheck validate plugin_to_validate").toAscii().data());
    mafMsgDebug("%s", mafTr("qaCheck profile  script_to_profile [parameters]").toAscii().data());
    mafMsgDebug("%s", mafTr("\n").toAscii().data());
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        mafCore::mafCoreSingletons::mafSingletonsInitialize();
        printUsage();
        exit(0);
    }

    mafString cmd(argv[1]);
    mafString param(argv[2]);

    if(cmd != "run" && cmd != "validate" && cmd != "profile") {
        printUsage();
        exit(0);
    }

    //QCoreApplication a(argc, argv);

    mafQAChecker checker;
    mafStringList argList;
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
