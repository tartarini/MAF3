/*
 *  mafLogger.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLogger.h"

using namespace mafCore;

mafLogger::mafLogger(const mafString code_location) : mafObjectBase(code_location), m_LogMode(mafLogModeAllMessages), m_Enabled(true) {
}

void mafLogger::setLogMode(const mafLogMode mode) {
    if(m_LogMode != mode) {
        m_LogMode = mode;
    }
}

void mafLogger::logMessage(const mafMsgType type, const mafString &msg) {
    REQUIRE(type == mafMsgTypeCritical ||
            type == mafMsgTypeDebug ||
            type == mafMsgTypeFatal ||
            type == mafMsgTypeWarning);

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
