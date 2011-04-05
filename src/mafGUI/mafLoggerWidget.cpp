/*
 *  mafLoggerWidget.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLoggerWidget.h"
#include "mafTextHighlighter.h"

using namespace mafCore;
using namespace mafGUI;

mafLoggerWidget::mafLoggerWidget(const QString code_location): mafLogger(code_location) {
    m_TextWidgetLog = new mafTextEditWidget();
    m_TextHighlighter = new mafTextHighlighter();

    QTextCharFormat format;
    format.setForeground(Qt::darkGreen);
    format.setFontWeight(QFont::Bold);
    QRegExp patternDebug("\\bDebug\\b");
    m_TextHighlighter->insertFormat("Debug", format);
    m_TextHighlighter->insertRule("Debug", patternDebug, m_TextHighlighter->format("Debug"));

    format.setForeground(Qt::darkYellow);
    format.setFontWeight(QFont::Bold);
    QRegExp patternWarning("\\bWarning\\b");
    m_TextHighlighter->insertFormat("Warning", format);
    m_TextHighlighter->insertRule("Warning", patternWarning, m_TextHighlighter->format("Warning"));

    format.setForeground(Qt::red);
    format.setFontWeight(QFont::Bold);
    QRegExp patternFatal("\\bFatal\\b");
    m_TextHighlighter->insertFormat("Fatal", format);
    m_TextHighlighter->insertRule("Fatal", patternFatal, m_TextHighlighter->format("Fatal"));

    format.setForeground(Qt::red);
    format.setFontWeight(QFont::Bold);
    QRegExp patternCritical("\\bCritical\\b");
    m_TextHighlighter->insertFormat("Critical", format);
    m_TextHighlighter->insertRule("Critical", patternCritical, m_TextHighlighter->format("Critical"));


    m_TextWidgetLog->setHighlighter(m_TextHighlighter);
}

mafLoggerWidget::~mafLoggerWidget() {
    delete m_TextHighlighter;
    m_TextHighlighter = NULL;
}

void mafLoggerWidget::clearLogHistory() {
    m_TextWidgetLog->clear();
}

void mafLoggerWidget::loggedMessage(const QtMsgType type, const QString &msg) {
    QString line("");
    line.append(QDateTime::currentDateTime().toString(mafDateTimeLogFormat));
    line.append(" --> ");
    switch (type) {
        case QtDebugMsg:
            if(logMode() == mafLogModeTestSuite) {
                line.append(TEST_SUITE_LOG_PREFIX);
            } else {
                line.append("Debug: ");
            }
        break;
        case QtWarningMsg:
            if(logMode() == mafLogModeTestSuite) {
                line.append(TEST_SUITE_LOG_PREFIX);
            } else {
                line.append("Warning: ");
            }
        break;
        case QtCriticalMsg: // System message types are defined as same type by Qt.
            line.append("Critical: ");
        break;
        case QtFatalMsg:
            line.append("Fatal: ");
        break;
    }
    line.append(msg);
    m_TextWidgetLog->append(line);
}

bool mafLoggerWidget::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        return (m_TextWidgetLog == ((mafLoggerWidget *)obj)->textWidgetLog());
    }
    return false;
}

