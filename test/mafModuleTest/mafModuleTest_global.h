/*
 *  mafModuleTest_Global.h
 *  mafModuleTest
 *
 *  Created by Paolo Quadrani on 27/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMODULETEST_GLOBAL_H
#define MAFMODULETEST_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(mafPluginTest_EXPORTS)
#  define MAFMODULETESTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFMODULETESTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFMODULETEST_GLOBAL_H
