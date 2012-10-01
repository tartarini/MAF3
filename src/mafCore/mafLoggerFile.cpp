/*
 *  mafLoggerFile.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLoggerFile.h"
#include <QDir>

using namespace mafCore;

mafLoggerFile::mafLoggerFile(const QString code_location) : mafLogger(code_location) {
    initializeNewTemporaryFile();
}

mafLoggerFile::~mafLoggerFile() {
    closeLastTempFile();
}

void mafLoggerFile::loggedMessage(const QtMsgType type, const QString &msg) {
    QString bufferLog = QDateTime::currentDateTime().toString(mafDateTimeLogFormat);
    bufferLog.append(" --> ");

    switch (type) {
        case QtDebugMsg:
            if(logMode() == mafLogModeTestSuite) {
                bufferLog.append(TEST_SUITE_LOG_PREFIX);
            } else {
                bufferLog.append("Debug: ");
            }
        break;
        case QtWarningMsg:
            if(logMode() == mafLogModeTestSuite) {
                bufferLog.append(TEST_SUITE_LOG_PREFIX);
            } else {
                bufferLog.append("Warning: ");
            }
        break;
        case QtCriticalMsg: // System message types are defined as same type by Qt.
            bufferLog.append("Critical: ");
        break;
        case QtFatalMsg:
            bufferLog.append("Fatal: ");
        break;
    }
    bufferLog.append(msg);
    bufferLog.append("\n");

    m_TempFileLog->write(bufferLog.toAscii());
}

void mafLoggerFile::clearLogHistory() {
    closeLastTempFile();
    QString old_file = m_LastLogFile;
    initializeNewTemporaryFile();
    QFile::remove(old_file);
}

void mafLoggerFile::initializeNewTemporaryFile() {
    m_TempFileLog = new QTemporaryFile();
    m_TempFileLog->setAutoRemove(false);
    QString tmp = QDir::tempPath();
    tmp.append("/maf3Logs");
    QDir log_dir(tmp);
    if(!log_dir.exists()) {
        log_dir.mkpath(tmp);
    }
    tmp.append("/log");
    m_TempFileLog->setFileTemplate(tmp);
    if(m_TempFileLog->open()) {
        m_LastLogFile = m_TempFileLog->fileName();
    } else {
        // Temporary file failed to open!
    }
}

void mafLoggerFile::closeLastTempFile() {
    m_TempFileLog->close();
    delete m_TempFileLog;
}

bool mafLoggerFile::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        if(this->m_TempFileLog == ((mafLoggerFile *)obj)->m_TempFileLog &&
            this->m_LastLogFile == ((mafLoggerFile *)obj)->m_LastLogFile) {
            return true;
        }
    }
    return false;
}
