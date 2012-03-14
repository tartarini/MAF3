/*==============================================================================

Program:   Multimod Application framework RELOADED
Module:    $RCSfile: vtkMAFPolyline2D.h,v $
Language:  C++
Date:      $Date: 2007/02/07 11:23:07 $
Version:   $Revision: 1.9 $
Authors:   Alexander Savenko, Nigel McFarlane, Paolo Quadrani

================================================================================
Copyright (c) 2002/2006 University of Bedfordshire, UK (www.beds.ac.uk)
All rights reserved.
===============================================================================*/

/* DESCRIPTION
vtkMAFPolyline2D - a mapper for direct rendering of isosurfaces &
a filter for extracting them.

vtkMAFPolyline2D performs a direct rendering of isosurfaces and 
extracts them as polydata objects if requested.
The input for the class can be either structured points (vtkImageData) or 
rectilinear grids (vtkRectilinearGrids). The optional output is vtkPolyData object.
This class can produce two kinds of surfaces: a standard isosurface and a 
simplified one that is obtained by skipping some of the voxels.
It is not guaranteed that the simplified model will have the same topology as the original one.
This mapper uses a special representation (min-max blocks) of the volume to speed up rendering. 
Additional acceleration is achieved by avoiding recalculation of vertices 
when the same edge is processed again during next iteration. 
This also solves the problem of locating identical points - the Achilles' heel 
of the standard VTK implementation of Marching Cubes algorithm.
*/

/* SEE ALSO
vtkVolumeMapper vtkContourFilter vtkMarchingCubes
*/

/* CONTENTS
namespace vtkPolyline2DNamespace
class vtkMAFPolyline2D
*/

/* PROGRAM FLOW
First call EstimateRelevantVolume()
Then Render()
*/

#ifndef __vtkPolyline2D_h
#define __vtkPolyline2D_h

#include "vtkMAF_global.h"

//------------------------------------------------------------------------------
// class vtkMAFPolyline2D
// these classes are used for optimizing the surface by analyzing 2D contours
//------------------------------------------------------------------------------
class vtkMAFPolyline2D 
{
public:
    /// 2D point with operators for ==, [], const [] and ()
    struct Point 
    {
        short xy[2];
        bool operator ==(const Point& operand) const 
        {
#ifdef WIN32
            return *((int*)xy) == *((int*)operand.xy);
#else
            return xy[0] == operand.xy[0] && xy[1] == operand.xy[1];
#endif
        }
        short& operator[](int index) { return xy[index]; }
        const short& operator[](int index) const { return xy[index]; }
        const short* operator()() const { return this->xy; }
    };

    // vertices
    int    start;
    int    end;
    Point *vertices;

    mutable short bbox[4]; // bounding box
    mutable bool  updateBoundingBox;

    float minDistance[2];
    int   closestPolyline[2];

public:
    vtkMAFPolyline2D(const Point *line);
    ~vtkMAFPolyline2D() { if (this->vertices != this->verticesBuffer) delete [] vertices; }

    int  Length() const   { return this->end -this->start + 1; }
    bool IsClosed() const { return (this->vertices[this->start] == this->vertices[this->end]); }

    // modifying polyline
    bool AddNextLine(const Point *line);
    bool Merge(vtkMAFPolyline2D &polyline);
    void Close();

    void UpdateBoundingBox() const;

    bool IsInsideOf(const vtkMAFPolyline2D *polyline) const;
    void FindClosestPolyline(int index, int numOfPolylines, vtkMAFPolyline2D* polylines);
    int  FindSubPolyline(int numOfPolylines, vtkMAFPolyline2D* polylines, float &minDistance);
    bool SplitPolyline(vtkMAFPolyline2D& subpoly, vtkMAFPolyline2D& newpoly);

    void Move(vtkMAFPolyline2D &polyline);

protected:
    int size;

    void Allocate(int newsize);
    void Reallocate();

#define VERTICES_BUFFER_SIZE 64
    Point verticesBuffer[VERTICES_BUFFER_SIZE];
};


#endif
