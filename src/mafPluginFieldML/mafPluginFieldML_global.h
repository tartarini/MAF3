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

#ifndef MAFPLUGINFIELDML_GLOBAL_H
#define MAFPLUGINFIELDML_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginFieldML_EXPORTS)
#  define MAFPLUGINFIELDMLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFPLUGINFIELDMLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFPLUGINFIELDML_GLOBAL_H
