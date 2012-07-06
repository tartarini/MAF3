/*
 *  mafPluginZip_Global.h
 *  mafPluginQt
 *
 *  Created by Matteo Giacomoni on 11/06/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINZIP_GLOBAL_H
#define MAFPLUGINZIP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginZip_EXPORTS)
#  define MAFPLUGINZIPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINZIPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINZIP_GLOBAL_H
