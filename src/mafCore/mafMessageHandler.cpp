/*
 *  mafMessageHandler.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMessageHandler.h"
#include "mafObjectFactory.h"
#include "mafLoggerConsole.h"

using namespace mafCore;

static void mafMessageHandlerOutput(QtMsgType type, const char *msg) {
    mafMessageHandler *handler = mafMessageHandler::instance();
    QString msg_to_forward = msg;
    mafLogger *current_logger = handler->activeLogger();
    mafLogger *secondary_logger = handler->secondaryLogger();
    if(current_logger->enabled()) {
        mafLogMode lm = current_logger->logMode();
        if(lm == mafLogModeOnlyCritical && type != QtCriticalMsg) {
            return;
        }
        if(lm == mafLogModeOnlyFatal && type != QtFatalMsg) {
            return;
        }
        if(lm == mafLogModeNoDebug && type == QtDebugMsg) {
            return;
        }
        if(lm == mafLogModeNoWarnings && type == QtWarningMsg) {
            return;
        }
        current_logger->logMessage(type, msg_to_forward);
        if (secondary_logger) {
            secondary_logger->logMessage(type, msg_to_forward);
        }
    }
}


mafMessageHandler::mafMessageHandler() : m_ActiveLogger(NULL), m_DefaultLogger(NULL), m_SecondaryLogger(NULL) {
    m_Lock = new QReadWriteLock(QReadWriteLock::Recursive);
}

mafMessageHandler::~mafMessageHandler() {
    delete m_Lock;
}

mafMessageHandler* mafMessageHandler::instance() {
    static mafMessageHandler instanceMessageHandler;
    return &instanceMessageHandler;
}

void mafMessageHandler::shutdown() {
    if(!m_OldMsgHandlerStack.isEmpty()) {
        qInstallMsgHandler(m_OldMsgHandlerStack.top());
        m_OldMsgHandlerStack.pop();
    } else {
        mafDEL(m_DefaultLogger);
        qInstallMsgHandler(0);
    }
}

void mafMessageHandler::installMessageHandler() {
    QtMsgHandler messageHandler = qInstallMsgHandler(mafMessageHandlerOutput);
    if(messageHandler) {
        m_OldMsgHandlerStack.push(messageHandler);
    }
}

mafLogger *mafMessageHandler::activeLogger() {
    if (m_ActiveLogger == NULL) {
        m_DefaultLogger = mafNEW(mafCore::mafLoggerConsole);
        setActiveLogger(m_DefaultLogger);
    }
    return m_ActiveLogger;
}

void mafMessageHandler::testSuiteLogMode(bool on) {
    if (on) {
        activeLogger()->setLogMode(mafLogModeTestSuite);
    } else {
        activeLogger()->setLogMode(m_LastLogMode);
    }
}

void mafMessageHandler::setActiveLogger(mafLogger *logger) {
    REQUIRE(logger != NULL);

    if(m_ActiveLogger != logger) {
        if(m_ActiveLogger) {
            // Reset the log mode for the previous logger.
            m_ActiveLogger->setLogMode(m_LastLogMode);
        }
        // Init the active logger
        m_ActiveLogger = logger;
        // ... and the last log mode variable.
        m_LastLogMode = m_ActiveLogger->logMode();
    }

    ENSURE(m_ActiveLogger != NULL);
}

void mafMessageHandler::setDeafultLoggerAsActive() {
    m_ActiveLogger = m_DefaultLogger;
}

void mafMessageHandler::setSecondaryLogger(mafLogger *logger) {
    mafDEL(m_SecondaryLogger);
    m_SecondaryLogger = logger;
    m_SecondaryLogger->retain();
}
