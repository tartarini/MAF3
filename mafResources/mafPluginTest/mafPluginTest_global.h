/*
 *  mafPluginVTK_Global.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINTEST_GLOBAL_H
#define MAFPLUGINTEST_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginTest_EXPORTS)
#  define MAFPLUGINTESTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINTESTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINTEST_GLOBAL_H
