/*
 *  mafPluginQt_Global.h
 *  mafPluginQt
 *
 *  Created by Paolo Quadrani on 19/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINQT_GLOBAL_H
#define MAFPLUGINQT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginQt_EXPORTS)
#  define MAFPLUGINQTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINQTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINQT_GLOBAL_H
