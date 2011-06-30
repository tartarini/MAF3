/*
 *  mafUtility.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFUTILITIES_H
#define MAFUTILITIES_H

#include <limits>
#include "mafCore_global.h"


#define EPSILON_DOUBLE std::numeric_limits<double>::epsilon()
#define EPSILON_FLOAT std::numeric_limits<float>::epsilon()


extern MAFCORESHARED_EXPORT bool mafEquals(double x, double y, double tolerance = EPSILON_DOUBLE);
extern MAFCORESHARED_EXPORT bool mafFloatEquals(float x, float y, float tolerance = EPSILON_FLOAT);
extern MAFCORESHARED_EXPORT double mafRoundToPrecision(double val, unsigned prec);
extern MAFCORESHARED_EXPORT bool mafIsLittleEndian(void);

#endif // MAFUTILITIES_H
