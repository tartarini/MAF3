/*==============================================================================

Program:   Multimod Application framework RELOADED
Module:    $RCSfile: vtkMAFListOfPolyline2D.h,v $
Language:  C++
Date:      $Date: 2007/02/07 11:23:07 $
Version:   $Revision: 1.9 $
Authors:   Alexander Savenko, Nigel McFarlane, Paolo Quadrani

================================================================================
Copyright (c) 2002/2006 University of Bedfordshire, UK (www.beds.ac.uk)
All rights reserved.
===============================================================================*/


#ifndef __vtkListOfPolyline2D_h
#define __vtkListOfPolyline2D_h

#include "vtkMAF_global.h"

#include <vector>


//------------------------------------------------------------------------------
// Forward declarations
//------------------------------------------------------------------------------
class vtkMAFPolyline2D;


//------------------------------------------------------------------------------
// class vtkMAFListOfPolyline2D
// This is a std::vector of polyline pointers, with 3 extra functions
//------------------------------------------------------------------------------
class vtkMAFListOfPolyline2D : public std::vector<vtkMAFPolyline2D*> 
{
public:
    void clear();                                                ///< Clear the list of polylines
    bool IsInside(int x, int y, int polylineLengthThreshold);
    vtkMAFPolyline2D *FindContour(int x, int y, int polylineLengthThreshold, int distance = 1);
};

#endif
