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

mafLoggerBuffer::mafLoggerBuffer(const QString code_location) : mafLogger(code_location) {
}

mafLoggerBuffer::~mafLoggerBuffer() {
}

void mafLoggerBuffer::loggedMessage(const QtMsgType type, const QString &msg) {
    m_BufferLog.append(QDateTime::currentDateTime().toString(mafDateTimeLogFormat));
    m_BufferLog.append(" --> ");
    switch (type) {
        case QtDebugMsg:
            if(logMode() == mafLogModeTestSuite) {
                m_BufferLog.append(TEST_SUITE_LOG_PREFIX);
            } else {
                m_BufferLog.append("Debug: ");
            }
        break;
        case QtWarningMsg:
            if(logMode() == mafLogModeTestSuite) {
                m_BufferLog.append(TEST_SUITE_LOG_PREFIX);
            } else {
                m_BufferLog.append("Warning: ");
            }
        break;
        case QtCriticalMsg: // System message types are defined as same type by Qt.
            m_BufferLog.append("Critical: ");
        break;
        case QtFatalMsg:
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
