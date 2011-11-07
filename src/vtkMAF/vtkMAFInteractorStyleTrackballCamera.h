/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMAFInteractorStyleTrackballCamera.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkMAFInteractorStyleTrackballCamera - manipulate camera in the scene independent of each other
// .SECTION Description
// vtkMAFInteractorStyleTrackballCamera allows the user to interact with (rotate,
// pan, etc.) camera in the scene indendent of each other.  In trackball
// interaction, the magnitude of the mouse motion is proportional to the
// actor motion associated with a particular mouse binding. For example,
// small left-button motions cause small changes in the rotation of the
// actor around its center point.
//
// The mouse bindings are as follows. For a 3-button mouse, the left button
// is for rotation, the right button for zooming, the middle button for
// panning, and ctrl + left button for spinning.  (With fewer mouse buttons,
// ctrl + shift + left button is for zooming, and shift + left button is for
// panning.)

// .SECTION See Also
// vtkInteractorStyleTrackballActor vtkInteractorStyleJoystickActor

#ifndef __vtkMAFInteractorStyleTrackballCamera_h
#define __vtkMAFInteractorStyleTrackballCamera_h

#include "vtkMAF_global.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>

class vtkCellPicker;
class vtkProp3D;
class vtkAssembly;

class VTK_vtkMAF_EXPORT vtkMAFInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera
{
public:
  static vtkMAFInteractorStyleTrackballCamera *New();
  vtkTypeMacro(vtkMAFInteractorStyleTrackballCamera,vtkInteractorStyleTrackballCamera);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Event bindings controlling the effects of pressing mouse buttons
  // or moving the mouse.
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnMiddleButtonDown();
  virtual void OnRightButtonDown();
  virtual void OnMouseWheelForward();
  virtual void OnMouseWheelBackward();

  // Description:
  // OnChar is triggered when an ASCII key is pressed. Some basic key presses
  // are handled here ('q' for Quit, 'p' for Pick, etc)
  /*virtual*/ void OnChar();

  vtkGetMacro(UseDefaultRenderer,int);
  vtkSetMacro(UseDefaultRenderer,int);
  vtkBooleanMacro(UseDefaultRenderer, int );

  void SetDefaultRenderer(vtkRenderer *ren);
    
protected:
  vtkMAFInteractorStyleTrackballCamera();
  ~vtkMAFInteractorStyleTrackballCamera();

  vtkRenderer *DefaultRenderer;
  int UseDefaultRenderer;

private:
  vtkMAFInteractorStyleTrackballCamera(const vtkMAFInteractorStyleTrackballCamera&);  // Not implemented.
  void operator=(const vtkMAFInteractorStyleTrackballCamera&);  // Not implemented.
};

#endif
