/*
 *  mafScriptInterpreterDefinitions.h
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
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
 
    
} //end namespace

#endif //MAFSCRIPTINTERPRETERDEFINITIONS_H
