/*
 *  mafPluginOutOfCore_Global.h
 *  mafPluginOutOfCore
 *
 *  Created by Daniele Giunchi && Roberto Mucci on 30/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINOUTOFCORE_GLOBAL_H
#define MAFPLUGINOUTOFCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginOutOfCore_EXPORTS)
#  define MAFPLUGINOUTOFCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINOUTOFCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINOUTOFCORE_GLOBAL_H
