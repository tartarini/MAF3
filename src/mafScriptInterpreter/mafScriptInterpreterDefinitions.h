/* mafGlobal.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Oct 16 09:54:33 2008 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Aug 24 12:47:35 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 111
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFSCRIPTINTERPRETERDEFINITIONS_H
#define MAFSCRIPTINTERPRETERDEFINITIONS_H

#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QThread>

namespace mafScriptInterpreter {

// /////////////////////////////////////////////////////////////////
// Output colors
// /////////////////////////////////////////////////////////////////

#define MAF_COLOR_FG_BLACK    "\033[00;30m"
#define MAF_COLOR_FG_RED      "\033[00;31m"
#define MAF_COLOR_FG_GREEN    "\033[00;32m"
#define MAF_COLOR_FG_YELLOW   "\033[00;33m"
#define MAF_COLOR_FG_BLUE     "\033[00;34m"
#define MAF_COLOR_FG_MAGENTA  "\033[00;35m"
#define MAF_COLOR_FG_CYAN     "\033[00;36m"
#define MAF_COLOR_FG_WHITE    "\033[00;37m"

#define MAF_COLOR_FG_LTBLACK   "\033[01;30m"
#define MAF_COLOR_FG_LTRED     "\033[01;31m"
#define MAF_COLOR_FG_LTGREEN   "\033[01;32m"
#define MAF_COLOR_FG_LTYELLOW  "\033[01;33m"
#define MAF_COLOR_FG_LTBLUE    "\033[01;34m"
#define MAF_COLOR_FG_LTMAGENTA "\033[01;35m"
#define MAF_COLOR_FG_LTCYAN    "\033[01;36m"
#define MAF_COLOR_FG_LTWHITE   "\033[01;37m"

#define MAF_COLOR_BG_BLACK    "\033[00;40m"
#define MAF_COLOR_BG_RED      "\033[00;41m"
#define MAF_COLOR_BG_GREEN    "\033[00;42m"
#define MAF_COLOR_BG_YELLOW   "\033[00;43m"
#define MAF_COLOR_BG_BLUE     "\033[00;44m"
#define MAF_COLOR_BG_MAGENTA  "\033[00;45m"
#define MAF_COLOR_BG_CYAN     "\033[00;46m"
#define MAF_COLOR_BG_WHITE    "\033[00;47m"

#define MAF_COLOR_FG_BD       "\033[01m"
#define MAF_COLOR_FG_UL       "\033[04m"
#define MAF_NO_COLOR          "\033[00m"

// /////////////////////////////////////////////////////////////////
// Default implementation warning
// /////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#  define MAF_PRETTY_FUNCTION __FUNCSIG__
#elif defined __GNUG__
#  define MAF_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else 
#  define MAF_PRETTY_FUNCTION __func__
#endif

#define MAF_DEFAULT_IMPLEMENTATION                                      \
    qDebug()                                                            \
    << "Using default implementation of"                                \
    << MAF_PRETTY_FUNCTION                                              \
    << "for"                                                            \
    << this->metaObject()->className()

#define MAF_UNUSED(variable) Q_UNUSED(variable)

#define MAF_DEPRECATED Q_DECL_DEPRECATED

#define MAF_SUCCEED 1
#define MAF_FAILURE 0
 
// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

inline QString mafReadFile(const QString& path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly))
        qDebug() << "Unable to read file" << path;

    QString contents = file.readAll();

    file.close();

    return contents;
}

inline bool mafApplicationArgumentsContain(int argc, char **argv, QString value)
{
    for(int i = 1; i < argc; i++)
        if(QString(argv[i]) == value)
            return true;

    return false;
}

inline bool mafApplicationArgumentsContain(QCoreApplication *application, QString value)
{
    return mafApplicationArgumentsContain(application->argc(), application->argv(), value); // I know the previous is deprecated but it really shouldn't :-|
}

inline QString mafApplicationArgumentsValue(int argc, char **argv, QString key)
{
    for(int i = 1; i < argc; i++)
        if(QString(argv[i]) == key)
            return QString(argv[i+1]);

    return QString();
}

inline QString mafApplicationArgumentsValue(QCoreApplication *application, QString key)
{
    return mafApplicationArgumentsValue(application->argc(), application->argv(), key); // I know the previous is deprecated but it really shouldn't :-|
}

// /////////////////////////////////////////////////////////////////
// Hash functions
// /////////////////////////////////////////////////////////////////

inline uint qHash(const QStringList &key)
{
    uint hash = 0;
    foreach(QString string, key)
	hash = hash ^ qHash(string);
    return hash;
}
    
} //end namespace

#endif //MAFSCRIPTINTERPRETERDEFINITIONS_H
