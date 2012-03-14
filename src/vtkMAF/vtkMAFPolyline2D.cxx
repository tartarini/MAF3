/*==============================================================================

Program:   Multimod Application framework RELOADED
Module:    $RCSfile: vtkMAFPolyline2D.cxx,v $
Language:  C++
Date:      $Date: 2007/02/09 15:51:45 $
Version:   $Revision: 1.17 $
Authors:   Alexander Savenko, Nigel McFarlane, Paolo Quadrani

================================================================================
Copyright (c) 2002/2006 University of Bedfordshire, UK (www.beds.ac.uk) 
All rights reserved.
===============================================================================*/

#include "vtkMAFPolyline2D.h"
#include <assert.h>
#include <string.h>

#include <vtkType.h>

//---------------------------------------------------------------------------------
void vtkMAFPolyline2D::Reallocate() {
    const int newsize = 2 * (size + 1);
    Point *buffer = new Point[newsize];
    const int newstart = int(0.5f * size + 1);
    const int newend   = newstart + this->end - this->start;
    memcpy(buffer + newstart, this->vertices + this->start, sizeof(Point) * (this->end - this->start + 1));
    if (this->vertices != this->verticesBuffer) {
        delete [] vertices;
    }
    this->vertices = buffer;
    this->start = newstart;
    this->end   = newend;
    this->size = newsize;
    assert(this->start > 1 && (this->end + 2) < newsize);
}


//---------------------------------------------------------------------------------
void vtkMAFPolyline2D::Allocate(int newsize) {
    if (this->vertices != NULL && (this->end - this->start) > newsize) {
        return;
    }
    if (this->vertices != this->verticesBuffer) {
        delete [] vertices;
    }
    this->vertices = new Point[newsize];
    this->start = -1;
    this->end   = -1;
    this->size  = newsize;
    this->closestPolyline[0] = this->closestPolyline[1] = -1;
    this->minDistance[0] = this->minDistance[1] = 0;
}


//---------------------------------------------------------------------------------
vtkMAFPolyline2D::vtkMAFPolyline2D(const Point *line) {
    assert(sizeof(Point) == (2 * sizeof(short)));
    this->size = VERTICES_BUFFER_SIZE;
    this->start = this->end = VERTICES_BUFFER_SIZE >> 1;
    this->end++;
    this->vertices = this->verticesBuffer;
    this->vertices[this->start] = line[0];
    this->vertices[this->end]   = line[1];
    this->closestPolyline[0] = this->closestPolyline[1] = -1;
    this->minDistance[0] = this->minDistance[1] = 0;
    this->updateBoundingBox = true;
}


//---------------------------------------------------------------------------------
bool vtkMAFPolyline2D::AddNextLine(const Point *newLine) {
    if (newLine[0] == this->vertices[this->start]) {
        if (this->start == 0) {
            Reallocate();
        }
        this->vertices[--this->start] = newLine[1];
    } else if (newLine[0] == this->vertices[this->end]) {
        if ((this->end + 1) == size) {
            Reallocate();
        }
        this->vertices[++this->end] = newLine[1];
    } else if (newLine[1] == this->vertices[this->start]) {
        if (this->start == 0) {
            Reallocate();
        }
        this->vertices[--this->start] = newLine[0];
    } else if (newLine[1] == this->vertices[this->end]) {
        if ((this->end + 1) == size) {
            Reallocate();
        }
        this->vertices[++this->end] = newLine[0];
    } else {
        return false;
    }
    assert(this->end < this->size && this->start >= 0);
    this->updateBoundingBox = true;
    return true;
}


//---------------------------------------------------------------------------------
bool vtkMAFPolyline2D::Merge(vtkMAFPolyline2D &polyline) {
    if (polyline.vertices[polyline.start] == this->vertices[start]) {
        while (this->start <= (polyline.end - polyline.start)) {
            Reallocate();
        }
        for (int i = polyline.start + 1; i <= polyline.end; i++) {
            this->vertices[--this->start] = polyline.vertices[i];
        }
    } else if (polyline.vertices[polyline.start] == this->vertices[end]) {
        while ((this->size - this->end) <= (polyline.end - polyline.start)) {
            Reallocate();
        }
        memcpy(this->vertices + end + 1, polyline.vertices + polyline.start + 1, sizeof(Point) * (polyline.end - polyline.start));
        this->end += polyline.end - polyline.start;
    } else if (polyline.vertices[polyline.end] == this->vertices[start]) {
        while (this->start <= (polyline.end - polyline.start)) {
            Reallocate();
        }
        memcpy(this->vertices + start - (polyline.end - polyline.start), 
            polyline.vertices + polyline.start, sizeof (Point) * (polyline.end - polyline.start));
        this->start -= polyline.end - polyline.start;
    } else if (polyline.vertices[polyline.end] == this->vertices[end]) {
        while ((this->size - this->end) <= (polyline.end - polyline.start)) {
            Reallocate();
        }
        for (int i = polyline.end - 1; i >= polyline.start; i--) {
            this->vertices[++this->end] = polyline.vertices[i];
        }
    } else {
        return false;
    }

    this->updateBoundingBox = true;
    assert(this->end < this->size && this->start >= 0);
    return true;
}


//---------------------------------------------------------------------------------
void vtkMAFPolyline2D::Close() {
    if (this->vertices[this->start] == this->vertices[this->end]) {
        if ((this->end + 1) == this->size) {
            this->Reallocate();
        }
        this->vertices[++this->end] = this->vertices[this->start];
    }
}



//---------------------------------------------------------------------------------
void vtkMAFPolyline2D::UpdateBoundingBox() const {
    this->bbox[0] = this->bbox[2] = VTK_SHORT_MAX;
    this->bbox[1] = this->bbox[3] = VTK_SHORT_MIN;
    for (int i = this->start; i <= this->end; i++) {
        const short * const xy = this->vertices[i].xy;
        if (xy[0] > this->bbox[1]) {
            this->bbox[1] = xy[0];
        }
        if (xy[0] < this->bbox[0]) {
            this->bbox[0] = xy[0];
        }
        if (xy[1] > this->bbox[3]) {
            this->bbox[3] = xy[1];
        }
        if (xy[1] < this->bbox[2]) {
            this->bbox[2] = xy[1];
        }
    }
    this->updateBoundingBox = false;
}


//------------------------------------------------------------------------------
void vtkMAFPolyline2D::Move(vtkMAFPolyline2D &polyline) {
    if (this->vertices != this->verticesBuffer) {
        delete [] vertices;
    }
    *this = polyline; // copy all members

    this->updateBoundingBox = true;

    // reset the source
    polyline.vertices = NULL;
}


//------------------------------------------------------------------------------
// check if one polyline is inside another       
bool vtkMAFPolyline2D::IsInsideOf(const vtkMAFPolyline2D *outerPolyline) const {
    if (this->updateBoundingBox) {
        this->UpdateBoundingBox();
    }
    if (outerPolyline->updateBoundingBox) {
        outerPolyline->UpdateBoundingBox();
    }

    // check bounding boxes
    if (outerPolyline->bbox[0] > this->bbox[1] ||
        outerPolyline->bbox[2] > this->bbox[3] ||
        outerPolyline->bbox[1] < this->bbox[0] ||
        outerPolyline->bbox[3] < this->bbox[2] || outerPolyline->Length() < 16) {
        return false;
    }

    // get sample point
    int intersection = 0;
    const short sx = this->vertices[this->start].xy[0];
    const short sy = this->vertices[this->start].xy[1];

    for (int p = outerPolyline->start + 1; p < outerPolyline->end; p++) {
        const short pointX   = outerPolyline->vertices[p].xy[0];
        const short pointY   = outerPolyline->vertices[p].xy[1];
        if (pointY != sy || pointX <= sx)
            continue;
        const short prevPointY   = outerPolyline->vertices[p - 1].xy[1];
        p++;
        while ((p < outerPolyline->end) && (outerPolyline->vertices[p].xy[1] == pointY)) {
            p++; // skip parallel lines
        }
        if (p < outerPolyline->end && outerPolyline->vertices[p].xy[1] != prevPointY) {
            intersection++;
        }
    }

    // check
    if ((intersection % 2) == 1 && this->Length() > 32) {
        intersection = 0;

        for (int p = outerPolyline->start + 1; p < outerPolyline->end; p++) {
            const short pointX   = outerPolyline->vertices[p].xy[0];
            const short pointY   = outerPolyline->vertices[p].xy[1];
            if (pointY != sy || pointX >= sx)
                continue;
            const short prevPointY   = outerPolyline->vertices[p - 1].xy[1];
            p++;
            while ((p < outerPolyline->end) && (outerPolyline->vertices[p].xy[1] == pointY)) {
                p++; // skip parallel lines
            }
            if (p < outerPolyline->end && outerPolyline->vertices[p].xy[1] != prevPointY) {
                intersection++;
            }
        }
    }

    return ((intersection % 2) == 1);
}
