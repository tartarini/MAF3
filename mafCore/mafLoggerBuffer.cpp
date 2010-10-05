/*
 *  mafLoggerBuffer.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLoggerBuffer.h"

using namespace mafCore;

mafLoggerBuffer::mafLoggerBuffer(const mafString code_location) : mafLogger(code_location) {
}

void mafLoggerBuffer::loggedMessage(const mafMsgType type, const mafString &msg) {
    m_BufferLog.append(mafDateTime::currentDateTime().toString(mafDateTimeLogFormat));
    m_BufferLog.append(" --> ");
    switch (type) {
        case mafMsgTypeDebug:
            if(logMode() == mafLogModeTestSuite) {
                m_BufferLog.append(TEST_SUITE_LOG_PREFIX);
            } else {
                m_BufferLog.append("Debug: ");
            }
        break;
        case mafMsgTypeWarning:
            if(logMode() == mafLogModeTestSuite) {
                m_BufferLog.append(TEST_SUITE_LOG_PREFIX);
            } else {
                m_BufferLog.append("Warning: ");
            }
        break;
        case mafMsgTypeCritical: // System message types are defined as same type by Qt.
            m_BufferLog.append("Critical: ");
        break;
        case mafMsgTypeFatal:
            m_BufferLog.append("Fatal: ");
        break;
    }
    m_BufferLog.append(msg);
    m_BufferLog.append("\n");
}

bool mafLoggerBuffer::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        return (m_BufferLog == ((mafLoggerBuffer *)obj)->bufferLog());
    }
    return false;
}
