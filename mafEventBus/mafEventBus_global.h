/*
 *  mafEventBus_global.h
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENTBUS_GLOBAL_H
#define MAFEVENTBUS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAFEVENTBUS_LIBRARY)
#  define MAFEVENTBUSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFEVENTBUSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFEVENTBUS_GLOBAL_H
