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
#include "mafSyntaxHighlighterMAFLogger.h"

using namespace mafCore;
using namespace mafGUI;

mafLoggerWidget::mafLoggerWidget(const QString code_location): mafLogger(code_location) {
    m_TextWidgetLog = new mafTextEditWidget();
    m_TextHighlighter = new mafSyntaxHighlighterMAFLogger();

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

