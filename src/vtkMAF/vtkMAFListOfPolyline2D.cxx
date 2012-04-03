/*==============================================================================

Program:   Multimod Application framework RELOADED
Module:    $RCSfile: vtkMAFListOfPolyline2D.cxx,v $
Language:  C++
Date:      $Date: 2007/02/09 15:51:45 $
Version:   $Revision: 1.17 $
Authors:   Alexander Savenko, Nigel McFarlane, Paolo Quadrani

================================================================================
Copyright (c) 2002/2006 University of Bedfordshire, UK (www.beds.ac.uk) 
All rights reserved.
===============================================================================*/

#include "vtkMAFListOfPolyline2D.h"
#include "vtkMAFPolyline2D.h"
#include <math.h>

//------------------------------------------------------------------------------
// Clear the list of polylines
void vtkMAFListOfPolyline2D::clear() 
//------------------------------------------------------------------------------
{
  for (int pj = size() - 1; pj >= 0; pj--) {
    delete at(pj);
	}
  erase(begin(), end());
}


//------------------------------------------------------------------------------
bool vtkMAFListOfPolyline2D::IsInside(int x, int y, int polylineLengthThreshold) {
  const short sx = x << 1;
  const short sy = y << 1;

  for (int pi = this->size() - 1; pi >= 0; pi--) {
    const vtkMAFPolyline2D * const polyline = at(pi);

    // ignore polyline if length < threshold
    if (polyline->Length() < polylineLengthThreshold) {
      continue;
	  }

    // check if bounding box needs updating
    if (polyline->updateBoundingBox) {
      polyline->UpdateBoundingBox();
	  }

    // check bounding boxes
    if ((polyline->bbox[0] > sx) || (polyline->bbox[1] < sx) ||
      (polyline->bbox[2]) > sy || (polyline->bbox[3] < sy)) {
      continue;
	  }

    int intersection = 0;
    for (int p = polyline->start + 1; p < polyline->end; p++) {
      const short pointX   = polyline->vertices[p].xy[0];
      const short pointY   = polyline->vertices[p].xy[1];
      if (pointY != sy || pointX <= sx) {
        continue;
		}
      const short prevPointY   = polyline->vertices[p - 1].xy[1];
      p++;
      while ((p < polyline->end) && (polyline->vertices[p].xy[1] == pointY)) {
        p++; // skip parallel lines
		}
      if (p < polyline->end && polyline->vertices[p].xy[1] != prevPointY) {
        intersection++;
		}
    }

    // check
    if ((intersection % 2) == 1) {
      intersection = 0;
      for (int p = polyline->start + 1; p < polyline->end; p++) {
        const short pointX   = polyline->vertices[p].xy[0];
        const short pointY   = polyline->vertices[p].xy[1];
        if (pointY != sy || pointX >= sx) {
          continue;
		  }
        const short prevPointY   = polyline->vertices[p - 1].xy[1];
        p++;
        while ((p < polyline->end) && (polyline->vertices[p].xy[1] == pointY)) {
          p++; // skip parallel lines
		  }
        if (p < polyline->end && polyline->vertices[p].xy[1] != prevPointY) {
          intersection++;
		  }
      }
    }

    if ((intersection % 2) == 1) {
      // test that the point is inside, not on the contour
      int p;
      for (p = polyline->start + 1; p < polyline->end; p++) {
        const short pointX   = polyline->vertices[p].xy[0];
        const short pointY   = polyline->vertices[p].xy[1];

        if (fabs((float)(pointX - sx)) <= 4 && fabs((float)(pointY - sy)) <= 4) {
          break;
		  }
      }

      if (p == polyline->end) {
        return true;
		}
    }
  }

  return false;
}



//------------------------------------------------------------------------------
vtkMAFPolyline2D *vtkMAFListOfPolyline2D::FindContour(int x, int y, int polylineLengthThreshold, int distance) {
  const short sx = x << 1;
  const short sy = y << 1;
  distance = distance << 1;

  for (int pi = this->size() - 1; pi >= 0; pi--) {
    vtkMAFPolyline2D *polyline = at(pi);
    if (polyline->Length() < polylineLengthThreshold) {
      continue;
	  }

    if (polyline->updateBoundingBox) {
      polyline->UpdateBoundingBox();
	  }

    // check bounding boxes
    if (polyline->bbox[0] > (sx + distance) || polyline->bbox[1] < (sx - distance) ||
      polyline->bbox[2] > (sy + distance) || polyline->bbox[3] < (sy - distance)) {
      continue;
	  }

    for (int p = polyline->start + 1; p < polyline->end; p++) {
      const short pointX   = polyline->vertices[p].xy[0];
      const short pointY   = polyline->vertices[p].xy[1];

      if (fabs((float)(pointX - sx)) <= distance && fabs((float)(pointY - sy)) <= distance) {
        return polyline;
		}
    }
  }

  return 0;
}
