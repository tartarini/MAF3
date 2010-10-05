/*
 *  mafContracts.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 15/06/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFCONTRACTS_H
#define MAFCONTRACTS_H

// Includes list
#include "mafCoreDefinitions.h"
#include <stdlib.h>

// Design by Contract macros
#ifdef QT_DEBUG
    #define ASSERT(bool_expression) if (!(bool_expression)) mafCore::AbortProgram(__FILE__, __LINE__, #bool_expression)
    #define IS_VALID(obj) ASSERT(obj != NULL && obj->isObjectValid())
    #define REQUIRE(bool_expression) ASSERT(bool_expression)
    #define ENSURE(bool_expression) ASSERT(bool_expression)
#else
    #define ASSERT(bool_expression)
    #define IS_VALID(obj)
    #define REQUIRE(bool_expression)
    #define ENSURE(bool_expression)
#endif

namespace mafCore {

inline void AbortProgram(const char *file, int line, const char *bool_expression) {
    mafMsgFatal("Program terminated in file: %s at line: %d on expression: %s", file, line, bool_expression);
}

} // mafCore

#endif // MAFCONTRACTS_
