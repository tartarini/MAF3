/*
 *  mafSerialization_global.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZATION_GLOBAL_H
#define MAFSERIALIZATION_GLOBAL_H

#include <QtCore/qglobal.h>
#include "mafSerializationDefinitions.h"

#if defined(mafSerialization_EXPORTS)
#  define MAFSERIALIZATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFSERIALIZATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFSERIALIZATION_GLOBAL_H
