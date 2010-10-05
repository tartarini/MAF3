/*
 *  mafResources_Global.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCES_GLOBAL_H
#define MAFRESOURCES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAFRESOURCES_LIBRARY)
#  define MAFRESOURCESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFRESOURCESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFRESOURCES_GLOBAL_H
