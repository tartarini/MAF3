/*
 *  mafLoggerSQLITE.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 07/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLoggerSQLITE.h"
#include <QDir>

using namespace mafCore;

mafLoggerSQLITE::mafLoggerSQLITE(const QString code_location) : mafLogger(code_location) {
    initializeNewTemporaryFile();
}

mafLoggerSQLITE::~mafLoggerSQLITE() {
    closeLastTempFile();
}

void mafLoggerSQLITE::loggedMessage(const QtMsgType type, const QString &msg) {
    QString bufferLog("log_");
    bufferLog.append(QDateTime::currentDateTime().toString("yyyy_MMM_dd_hh_mm"));
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

void mafLoggerSQLITE::clearLogHistory() {
    closeLastTempFile();
    QString old_file = m_LastLogFile;
    initializeNewTemporaryFile();
    QFile::remove(old_file);
}

void mafLoggerSQLITE::initializeNewTemporaryFile() {
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

void mafLoggerSQLITE::closeLastTempFile() {
    m_TempFileLog->close();
    delete m_TempFileLog;
}

bool mafLoggerSQLITE::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        if(this->m_TempFileLog == ((mafLoggerSQLITE *)obj)->m_TempFileLog &&
            this->m_LastLogFile == ((mafLoggerSQLITE *)obj)->m_LastLogFile) {
            return true;
        }
    }
    return false;
}
