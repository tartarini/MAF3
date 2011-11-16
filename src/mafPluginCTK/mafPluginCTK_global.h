/*
 *  mafPluginCTK_Global.h
 *  mafPluginCTK
 *
 *  Created by Daniele Giunchi && Roberto Mucci on 30/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINCTK_GLOBAL_H
#define MAFPLUGINCTK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginCTK_EXPORTS)
#  define MAFPLUGINCTKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINCTKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINCTK_GLOBAL_H
