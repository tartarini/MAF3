/*
 *  mafLogger.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLogger.h"

using namespace mafCore;

mafLogger::mafLogger(const QString code_location) : mafObjectBase(code_location), m_LogMode(mafLogModeAllMessages), m_Enabled(true) {
}

mafLogger::~mafLogger() {
}

void mafLogger::setLogMode(const mafLogMode mode) {
    if(m_LogMode != mode) {
        m_LogMode = mode;
    }
}

void mafLogger::logMessage(const QtMsgType type, QString &msg) {
    REQUIRE(type == QtCriticalMsg ||
            type == QtDebugMsg ||
            type == QtFatalMsg ||
            type == QtWarningMsg);

//    if(type == QtCriticalMsg && m_LogMode != mafLogModeTestSuite) {
//        QString failString(mafTr("FAIL! "));
//        msg.prepend(failString);
//    }
    loggedMessage(type, msg);
}

bool mafLogger::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        if(this->enabled() == ((mafLogger *)obj)->enabled() &&
            this->logMode() == ((mafLogger *)obj)->logMode()) {
            return true;
        }
    }
    return false;
}
