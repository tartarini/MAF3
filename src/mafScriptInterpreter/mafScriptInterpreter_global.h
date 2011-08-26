/*
 *  mafScriptInterpreter_global.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTINTERPRETER_GLOBAL_H
#define MAFSCRIPTINTERPRETER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafScriptInterpreter_EXPORTS)
#  define MAFSCRIPTINTERPRETERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFSCRIPTINTERPRETERSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "mafScriptInterpreterDefinitions.h"

#endif // MAFSCRIPTINTERPRETER_GLOBAL_H
