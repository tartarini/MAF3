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

mafLoggerConsole::mafLoggerConsole(const mafString code_location) : mafLogger(code_location) {
}

mafLoggerConsole::~mafLoggerConsole() {
}

void mafLoggerConsole::loggedMessage(const mafMsgType type, const mafString &msg) {
    std::cout << mafDateTime::currentDateTime().toString(mafDateTimeLogFormat).toStdString() << " --> ";
    switch (type) {
    case mafMsgTypeDebug:
        if(logMode() == mafLogModeTestSuite) {
            std::cout << TEST_SUITE_LOG_PREFIX << msg.toStdString() << std::endl;
        } else {
            std::cout << "Debug: " << msg.toStdString() << std::endl;
        }
        break;
    case mafMsgTypeWarning:
        if(logMode() == mafLogModeTestSuite) {
            std::cout << TEST_SUITE_LOG_PREFIX << msg.toStdString() << std::endl;
        } else {
        std::cout << "Warning: " << msg.toStdString() << std::endl;
        }
        break;
    case mafMsgTypeCritical:
        std::cout << "Critical: " << msg.toStdString() << std::endl;
        break;
    case mafMsgTypeFatal:
        std::cout << "Fatal: " << msg.toStdString() << std::endl;
        abort();
    }
}
