/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMAFImplicitPlaneRepresentation.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkMAFImplicitPlaneRepresentation - a class defining the representation for a vtkImplicitPlaneWidget2
// .SECTION Description
// This class is a concrete representation for the
// vtkImplicitPlaneWidget2. It represents an infinite plane defined by a
// normal and point in the context of a bounding box. Through interaction
// with the widget, the plane can be manipulated by adjusting the plane
// normal or moving the origin point.
//
// To use this representation, you normally define a (plane) origin and (plane)
// normal. The PlaceWidget() method is also used to intially position the
// representation.

// .SECTION Caveats
// This class, and vtkImplicitPlaneWidget2, are next generation VTK
// widgets. An earlier version of this functionality was defined in the
// class vtkImplicitPlaneWidget.

// .SECTION See Also
// vtkImplicitPlaneWidget2 vtkImplicitPlaneWidget


#ifndef __vtkMAFImplicitPlaneRepresentation_h
#define __vtkMAFImplicitPlaneRepresentation_h

#include "vtkMAF_global.h"
#include <vtkImplicitPlaneRepresentation.h>

class VTK_vtkMAF_EXPORT vtkMAFImplicitPlaneRepresentation : public vtkImplicitPlaneRepresentation
{
public:
  // Description:
  // Instantiate the class.
  static vtkMAFImplicitPlaneRepresentation *New();

  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Enable/Disable rotation interaction capability for the widget.
  void SetRotationEnabled(int enable);
  vtkGetMacro(RotationEnabled,int);
  vtkBooleanMacro(RotationEnabled,int);

  // Description:
  // Enable/Disable the drawing of the Outline BoundingBox. To disable also its interaction use the OutlineTranslationOn/Off method.
  void SetDrawOutlineBox(int box);
  vtkGetMacro(DrawOutlineBox,int);
  vtkBooleanMacro(DrawOutlineBox,int);

  // Description:
  // Methods to interface with the vtkImplicitPlaneWidget.
  virtual int ComputeInteractionState(int X, int Y, int modify=0);

  // Description:
  // Methods supporting the rendering process.
  virtual int RenderOpaqueGeometry(vtkViewport*);
  virtual int RenderTranslucentPolygonalGeometry(vtkViewport*);
  virtual int HasTranslucentPolygonalGeometry();

protected:
  vtkMAFImplicitPlaneRepresentation();
  ~vtkMAFImplicitPlaneRepresentation();

  int RotationEnabled;
  int DrawOutlineBox;

private:
  vtkMAFImplicitPlaneRepresentation(const vtkMAFImplicitPlaneRepresentation&);  //Not implemented
  void operator=(const vtkMAFImplicitPlaneRepresentation&);  //Not implemented
};

#endif
