/*
 *  mafMessageHandler.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMessageHandler.h"
#include "mafObjectFactory.h"
#include "mafLoggerConsole.h"

using namespace mafCore;

static void mafMessageHandlerOutput(mafMsgType type, const char *msg) {
    mafMessageHandler *handler = mafMessageHandler::instance();
    mafString msg_to_forward = msg;
    mafLogger *current_logger = handler->activeLogger();
    if(current_logger->enabled()) {
        mafLogMode lm = current_logger->logMode();
        if(lm == mafLogModeOnlyCritical && type != mafMsgTypeCritical) {
            return;
        }
        if(lm == mafLogModeOnlyFatal && type != mafMsgTypeFatal) {
            return;
        }
        if(lm == mafLogModeNoDebug && type == mafMsgTypeDebug) {
            return;
        }
        if(lm == mafLogModeNoWarnings && type == mafMsgTypeWarning) {
            return;
        }
        current_logger->logMessage(type, msg_to_forward);
    }
}


mafMessageHandler::mafMessageHandler() : m_OldMsgHandler(0), m_ActiveLogger(NULL) {
    m_DefaultLogger = mafNEW(mafCore::mafLoggerConsole);
    setActiveLogger(m_DefaultLogger);
}

mafMessageHandler::~mafMessageHandler() {
}

mafMessageHandler* mafMessageHandler::instance() {
    static mafMessageHandler instanceMessageHandler;
    return &instanceMessageHandler;
}

void mafMessageHandler::shutdown() {
    delete m_DefaultLogger;
    m_DefaultLogger = NULL;
    qInstallMsgHandler(m_OldMsgHandler);
}

void mafMessageHandler::installMessageHandler() {
    m_OldMsgHandler = qInstallMsgHandler(mafMessageHandlerOutput);
}

void mafMessageHandler::testSuiteLogMode(bool on) {
    if (on) {
        m_ActiveLogger->setLogMode(mafLogModeTestSuite);
    } else {
        m_ActiveLogger->setLogMode(m_LastLogMode);
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
