/*
 *  mafScriptInterpreter_global.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTINTERPRETER_GLOBAL_H
#define MAFSCRIPTINTERPRETER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafScriptEditor_EXPORTS)
#  define MAFSCRIPTINTERPRETERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFSCRIPTINTERPRETERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFGUI_GLOBAL_H
