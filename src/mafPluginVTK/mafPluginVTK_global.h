/*
 *  mafPluginVTK_Global.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINVTK_GLOBAL_H
#define MAFPLUGINVTK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginVTK_EXPORTS)
#  define MAFPLUGINVTKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINVTKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINVTK_GLOBAL_H
