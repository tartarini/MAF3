/*
 *  mafQA_global.h
 *  mafQA
 *
 *  Created by Paolo Quadrani on 21/08/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFQA_GLOBAL_H
#define MAFQA_GLOBAL_H

#include <QtCore/qglobal.h>
#include "mafCoreDefinitions.h"

#if defined(mafQA_EXPORTS)
#  define MAFQASHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAFQASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAFQA_GLOBAL_H

#include "mafQADefinitions.h"
