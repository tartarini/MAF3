/*
 *  mafCore_global.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCORE_GLOBAL_H
#define MAFCORE_GLOBAL_H

// Includes list
#include <QtCore/qglobal.h>

#if defined(mafCore_EXPORTS)
#  define MAFCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFCORE_GLOBAL_H

#include "mafCoreDefinitions.h"
