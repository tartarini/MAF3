/*
 *  mafUtilities.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 16/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafUtilities.h"
#include <cmath>

/// Check if two double values are equals
bool mafEquals(double x, double y, double tolerance) {
   return std::fabs(x - y) < tolerance;
}

/// Check if two float values are equals.
bool mafFloatEquals(float x, float y, float tolerance) {
   return std::fabs(x - y) < tolerance;
}

/// Truncate a double value to a given precision.
double mafRoundToPrecision(double val, unsigned prec) {
  double k = pow((double)10,(double)prec);
  return floor(val*k+0.5)/k;
}

/// Check the endianity of the current system
bool mafIsLittleEndian(void) {
  short magic, test;
  char * ptr;
  magic = 0xABCD;                     /* endianity test number */
  ptr = (char *) &magic;
  test = (ptr[1]<<8) + (ptr[0]&0xFF); /* build value byte by byte */
  return (magic == test);             /* if the same is little endian */
}
