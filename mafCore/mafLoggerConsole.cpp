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
    std::cout << QDateTime::currentDateTime().toString(mafDateTimeLogFormat).toStdString() << " --> ";
    switch (type) {
    case QtDebugMsg:
        if(logMode() == mafLogModeTestSuite) {
            std::cout << TEST_SUITE_LOG_PREFIX << msg.toStdString() << std::endl;
        } else {
            std::cout << "Debug: " << msg.toStdString() << std::endl;
        }
        break;
    case QtWarningMsg:
        if(logMode() == mafLogModeTestSuite) {
            std::cout << TEST_SUITE_LOG_PREFIX << msg.toStdString() << std::endl;
        } else {
        std::cout << "Warning: " << msg.toStdString() << std::endl;
        }
        break;
    case QtCriticalMsg:
        std::cout << "Critical: " << msg.toStdString() << std::endl;
        break;
    case QtFatalMsg:
        std::cout << "Fatal: " << msg.toStdString() << std::endl;
        abort();
    }
}
