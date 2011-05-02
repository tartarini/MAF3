/* mafLog.cpp --- 
 * 
 * Author: Jean-Christophe Lombardo
 * Copyright (C) 2008-2009 - Jean-Christophe Lombardo, Inria.
 * Created: Thu May 14 14:32:46 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Mar  3 19:18:12 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 121
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "mafLog.h"
// /////////////////////////////////////////////////////////////////
// mafStandardRedirector
// /////////////////////////////////////////////////////////////////

#include <iostream>
#include <streambuf>
#include <string>


using namespace mafScriptInterpreter;

QMultiMap<QString, mafLog::Handler> mafLog::s_handlers;


class mafStandardRedirector : public std::basic_streambuf<char>
{
public:
    enum Channel { Out, Err, Log } ;

     mafStandardRedirector(std::ostream &stream, Channel channel);
    ~mafStandardRedirector(void);
	
    static bool   initialize(void);
    static void uninitialize(void);

protected:
    virtual int overflow(int v);    
    virtual std::streamsize xsputn(const char *p, std::streamsize n);

    void flush(void);

private:
    std::ostream&   m_stream;
    std::streambuf *m_buffer;
    std::string     m_string;
    Channel         m_channel;

    static mafStandardRedirector *s_err;
    static mafStandardRedirector *s_out;
    static mafStandardRedirector *s_log;
};

mafStandardRedirector *mafStandardRedirector::s_err = NULL;
mafStandardRedirector *mafStandardRedirector::s_out = NULL;
mafStandardRedirector *mafStandardRedirector::s_log = NULL;

mafStandardRedirector::mafStandardRedirector(std::ostream &stream, Channel channel) : m_stream(stream), m_channel(channel)
{
    m_string = " ";
    m_buffer = stream.rdbuf();
    stream.rdbuf(this);
}

mafStandardRedirector::~mafStandardRedirector(void)
{
    if (!m_string.empty()) flush();

    m_stream.rdbuf(m_buffer);
}

void mafStandardRedirector::flush(void)
{
    switch(m_channel) {
    case Err: 
        mafLog::error() << QString::fromStdString(m_string);
        break;
    case Out: 
    case Log: 
        mafLog::output() << QString::fromStdString(m_string);
        break;
    default:
        break;
    };
}

int mafStandardRedirector::overflow(int v)
{
    if (v == '\n') {
        mafLog::output() << m_string.c_str();
        m_string.erase(m_string.begin(), m_string.end());
    } else
        m_string += v;
    
    return v;
}

std::streamsize mafStandardRedirector::xsputn(const char *p, std::streamsize n)
{
    if(p)
        m_string += QString::fromAscii(p, n).remove('\n').remove('\r').simplified().toStdString();

    return n;
}

bool mafStandardRedirector::initialize(void)
{
    if (!s_err) s_err = new mafStandardRedirector(std::cerr, mafStandardRedirector::Err);
    if (!s_out) s_out = new mafStandardRedirector(std::cout, mafStandardRedirector::Out);
    if (!s_log) s_log = new mafStandardRedirector(std::clog, mafStandardRedirector::Log);
    return true;
}

void mafStandardRedirector::uninitialize(void)
{
    if (s_err) { delete s_err; s_err = NULL; }
    if (s_out) { delete s_out; s_out = NULL; }
    if (s_log) { delete s_log; s_log = NULL; }
}

static bool log_initialized = mafStandardRedirector::initialize();

// /////////////////////////////////////////////////////////////////
// mafLog
// /////////////////////////////////////////////////////////////////

mafLog& mafLog::operator<<(int value)
{
    m_log += QString::number(value);
    
    return *this;
}

mafLog& mafLog::operator<<(float value)
{
    m_log += QString::number(value);

    return *this;
}

mafLog& mafLog::operator<<(double value)
{
    m_log += QString::number(value);

    return *this;
}

mafLog& mafLog::operator<<(const QString& value)
{ 
    m_log += value;

    return *this;
}

mafLog mafLog::output(const QString& source)
{
    return mafLog(source, Output);
}

mafLog mafLog::error(const QString& source)
{
    return mafLog(source, Error);
}

mafLog mafLog::debug(const QString& source)
{
    return mafLog(source, Debug);
}

mafLog mafLog::warning(const QString& source)
{
    return mafLog(source, Warning);
}

mafLog mafLog::critical(const QString& source)
{
    return mafLog(source, Critical);
}

mafLog mafLog::fatal(const QString& source)
{
    return mafLog(source, Fatal);
}

mafLog::mafLog(const QString& source, Level level) : m_level(level), m_source(source)
{

}

mafLog::~mafLog(void)
{
     if (s_handlers.keys().contains(m_source))
         foreach(Handler handler, s_handlers.values(m_source)) handler(m_level, m_log);
     else
         qDebug() << m_log.toAscii().constData();

     // qDebug().nospace() << QDate::currentDate().toString().toAscii().constData() << QTime::currentTime().toString().toAscii() << "(" << m_source << ") Level=" << m_level << "-" << m_log.toAscii();
}

void mafLog::registerHandler(Handler handler, const QString& source)
{
    s_handlers.insert(source, handler);
}

void mafLog::unregisterHandler(Handler handler, const QString& source)
{
    s_handlers.remove(source, handler);
}

void mafLog::disableRedirection(void)
{
    mafStandardRedirector::uninitialize();

    s_handlers.clear();
}

// /////////////////////////////////////////////////////////////////
// mafLogEvent
// /////////////////////////////////////////////////////////////////

mafLogEvent::mafLogEvent(mafLog::Level level, const QString& message) : QEvent(s_type)
{
    this->m_level = level;
    this->m_message = message;
}

mafLog::Level mafLogEvent::level(void) const
{
    return m_level;
}

QString mafLogEvent::message(void) const
{
    return m_message;
}

QEvent::Type mafLogEvent::s_type = static_cast<Type>(QEvent::registerEventType());

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

 mafLog mafOutput(void)
{
    return mafLog::output();
}

 mafLog mafError(void)
{
    return mafLog::error();
}

 mafLog mafDebug(void)
{
    return mafLog::debug();
}

 mafLog mafWarning(void)
{
    return mafLog::warning();
}

 mafLog mafCritical(void)
{
    return mafLog::critical();
}

 mafLog mafFatal(void)
{
    return mafLog::fatal();
}
