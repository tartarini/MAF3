/* mafLog.h --- 
 * 
 * Author: Jean-Christophe Lombardo
 * Copyright (C) 2008-2009 - Jean-Christophe Lombardo, Inria.
 * Created: Thu May 14 14:02:22 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Mon Feb 28 21:09:40 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 88
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFLOG_H
#define MAFLOG_H

#include "mafScriptInterpreter_global.h"
#include <QtCore>

namespace mafScriptInterpreter {

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafLog
{
public:
    enum LogType { Output, Error, Debug, Warning, Critical, Fatal };

    typedef void (*Handler)(LogType type, const QString &message);

    mafLog& operator<<(int value);
    mafLog& operator<<(float value);
    mafLog& operator<<(double value);
    mafLog& operator<<(const QString& value);

    ~mafLog(void);

    static mafLog   output(const QString& source = "default");
    static mafLog    error(const QString& source = "default");
    static mafLog    debug(const QString& source = "default");
    static mafLog  warning(const QString& source = "default");
    static mafLog critical(const QString& source = "default");
    static mafLog    fatal(const QString& source = "default");

    static void   registerHandler(Handler handler, const QString& source = "default");
    static void unregisterHandler(Handler handler, const QString& source = "default");

    static void disableRedirection(void);

private:
     mafLog(const QString& source, LogType logType);

private:
    LogType   m_LogType;
    QString m_source;
    QString m_log;

private:
    static QMultiMap<QString, Handler> s_handlers;
};

// /////////////////////////////////////////////////////////////////
// mafLogEvent
// /////////////////////////////////////////////////////////////////

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafLogEvent : public QEvent
{
public:
    mafLogEvent(mafLog::LogType logType, const QString& msg);
    
    mafLog::LogType logType(void) const;

    QString message(void) const;

private:
    mafLog::LogType m_LogType;

    QString m_message;

private:
    static Type s_type;
};

} //end namespace
    
#endif
