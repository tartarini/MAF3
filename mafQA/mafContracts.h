/*
 *  mafContracts.h
 *  mafQA
 *
 *  Created by Paolo Quadrani on 15/06/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef mafDebug_
#define mafDebug_

#include <stdlib.h>
#include "mafDefines.h"

// Design by Contract macros
#ifdef MAF3_DEBUG
    #define ASSERT(bool_expression) if (!(bool_expression)) AbortProgram(__FILE__, __LINE__, #bool_expression)
    #define IS_VALID(obj) ASSERT(obj != NULL && obj->IsValid())
    #define REQUIRE(bool_expression) ASSERT(bool_expression)
    #define ENSURE(bool_expression) ASSERT(bool_expression)
#else
    #define ASSERT(bool_expression)
    #define IS_VALID(obj)
    #define REQUIRE(bool_expression)
    #define ENSURE(bool_expression)
#endif

inline void AbortProgram(const char *file, int line, const char *bool_expression)
{
    std::cout << _T("Program terminated in file: ") << file << _T(" at line: ") << line << _T(" on expression: ") << bool_expression << std::endl;
    abort();
}

#endif
