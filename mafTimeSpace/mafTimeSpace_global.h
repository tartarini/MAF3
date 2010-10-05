/*
 *  mafTimeSpace_global.h
 *  mafTimeSpace
 *
 *  Created by Daniele Giunchi on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTIMESPACE_GLOBAL_H
#define MAFTIMESPACE_GLOBAL_H

#include <QtCore/qglobal.h>
#include "mafTimeSpaceDefinitions.h"

#if defined(MAFTIMESPACE_LIBRARY)
#  define MAFTIMESPACESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFTIMESPACESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFTIMESPACE_GLOBAL_H
