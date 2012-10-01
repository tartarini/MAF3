/*
 *  mafGUI_global.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUI_GLOBAL_H
#define MAFGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafGUI_EXPORTS)
#  define MAFGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFGUI_GLOBAL_H
