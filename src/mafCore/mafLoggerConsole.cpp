/*
 *  mafLoggerConsole.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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
	QString dt(QDateTime::currentDateTime().toString(mafDateTimeLogFormat));
	std::string cp; 
	cp.append(QDateTime::currentDateTime().toString(mafDateTimeLogFormat).toLatin1());
	cp.append(" ---> ");
	std::cout << cp << " " << "---> ";
	std::string message;
	message.append(msg.toLatin1());
    switch (type) {
    case QtDebugMsg:
        if(logMode() == mafLogModeTestSuite) {
			std::cout << TEST_SUITE_LOG_PREFIX << " " << message << std::endl;
        } else {
			std::cout << "Debug:" << message << std::endl;
        }
        break;
    case QtWarningMsg:
        if(logMode() == mafLogModeTestSuite) {
			std::cout << TEST_SUITE_LOG_PREFIX << " " << message << std::endl;
        } else {
			std::cout << "Warning:" << message << std::endl;
        }
        break;
    case QtCriticalMsg:
		std::cout << "Critical:" << message << std::endl;
        break;
    case QtFatalMsg:
		std::cout << "Fatal:" << message << std::endl;
        //abort();
        break;
    }
}
