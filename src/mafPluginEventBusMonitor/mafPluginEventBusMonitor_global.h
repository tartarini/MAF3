/*
 *  mafPluginEventBusMonitor_Global.h
 *  mafPluginEventBusMonitor
 *
 *  Created by Daniele Giunchi on 30/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINEVENTBUSMONITOR_GLOBAL_H
#define MAFPLUGINEVENTBUSMONITOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAFPLUGINEVENTBUSMONITOR_EXPORTS)
#  define MAFPLUGINEVENTBUSMONITORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINEVENTBUSMONITORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINEVENTBUSMONITOR_GLOBAL_H