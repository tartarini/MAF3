/*
 *  mafLoggerConsole.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLoggerConsole.h"
#include <iostream>

using namespace mafCore;

mafLoggerConsole::mafLoggerConsole(const QString code_location) : mafLogger(code_location) {
}

mafLoggerConsole::~mafLoggerConsole() {
}

void mafLoggerConsole::loggedMessage(const QtMsgType type, const QString &msg) {
    printf("%s ---> " , QDateTime::currentDateTime().toString(mafDateTimeLogFormat).toStdString().c_str());
    switch (type) {
    case QtDebugMsg:
        if(logMode() == mafLogModeTestSuite) {
            printf("%s %s\n", TEST_SUITE_LOG_PREFIX ,msg.toStdString().c_str());
        } else {
            printf("Debug: %s\n", msg.toStdString().c_str());
        }
        break;
    case QtWarningMsg:
        if(logMode() == mafLogModeTestSuite) {
            printf("%s %s\n", TEST_SUITE_LOG_PREFIX ,msg.toStdString().c_str());
        } else {
        printf("Warning: %s\n", msg.toStdString().c_str());    
        }
        break;
    case QtCriticalMsg:
        printf("Critical: %s\n", msg.toStdString().c_str());    
        break;
    case QtFatalMsg:
        printf("Fatal: %s\n", msg.toStdString().c_str());    
        abort();
    }
}
