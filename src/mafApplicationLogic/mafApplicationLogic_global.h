/*
 *  mafApplicationLogic_global.h
 *  mafApplicationLogic
 *
 *  Created by Daniele Giunchi on 28/06/10.
 *  Copyright 2009-2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFAPPLICATIONLOGIC_GLOBAL_H
#define MAFAPPLICATIONLOGIC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafApplicationLogic_EXPORTS)
#  define MAFAPPLICATIONLOGICSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFAPPLICATIONLOGICSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFAPPLICATIONLOGIC_GLOBAL_H
