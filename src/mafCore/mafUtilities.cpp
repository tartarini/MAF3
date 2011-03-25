/*
 *  mafUtilities.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 16/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafUtilities.h"
#include <math.h>

/// Check if two double values are equals
bool mafEquals(double x, double y) {
   double diff=fabs(x - y);
   double max_err=fabs(x / pow((double)10,(double)15));
   if (diff > max_err)
     return false;
   return  ( diff <= max_err);
}

/// Check if two float values are equals.
bool mafFloatEquals(float x, float y) {
   float diff=fabs(x - y);
   float max_err=fabs(x / pow((float)10,(float)7));
   return  ( diff <= max_err);
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
