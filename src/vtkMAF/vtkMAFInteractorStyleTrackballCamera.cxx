/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMAFInteractorStyleTrackballCamera.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMAFInteractorStyleTrackballCamera.h"

#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkAssembly.h>
#include <vtkAssemblyPath.h>
#include <vtkProp3D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkTransform.h>
#include <vtkProp3DCollection.h>
#include <vtkProperty.h>

vtkStandardNewMacro(vtkMAFInteractorStyleTrackballCamera);

//----------------------------------------------------------------------------
vtkMAFInteractorStyleTrackballCamera::vtkMAFInteractorStyleTrackballCamera() 
{
  this->MotionFactor    = 10.0;
  this->DefaultRenderer = NULL;
  this->UseDefaultRenderer = 0;
}

//----------------------------------------------------------------------------
vtkMAFInteractorStyleTrackballCamera::~vtkMAFInteractorStyleTrackballCamera() 
{
    SetDefaultRenderer(NULL);
}

void vtkMAFInteractorStyleTrackballCamera::SetDefaultRenderer(vtkRenderer *ren) {
    if (ren != this->DefaultRenderer) {
        if (this->DefaultRenderer)
        {
            this->DefaultRenderer->UnRegister(NULL);
        }
        this->DefaultRenderer = ren;
        if (this->DefaultRenderer)
        {
            this->DefaultRenderer->Register(NULL);
        }
        this->SetCurrentRenderer(DefaultRenderer);
    }
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnMouseMove() 
{
//  int x = this->Interactor->GetEventPosition()[0];
//  int y = this->Interactor->GetEventPosition()[1];

  switch (this->State) 
    {
    case VTKIS_ROTATE:
//      this->FindInteractiveRenderer(x, y);
      this->Rotate();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_PAN:
//      this->FindInteractiveRenderer(x, y);
      this->Pan();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_DOLLY:
//      this->FindInteractiveRenderer(x, y);
      this->Dolly();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_SPIN:
//      this->FindInteractiveRenderer(x, y);
      this->Spin();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    }
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnLeftButtonDown() 
{
	if(this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL) {
		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
	}
	
	if (this->CurrentRenderer == NULL)
    {
		return;
    }
  
	this->GrabFocus(this->EventCallbackCommand);
	if (this->Interactor->GetShiftKey()) 
    {
		if (this->Interactor->GetControlKey()) 
		{
			this->StartDolly();
		}
		else 
		{
			this->StartPan();
		}
    } 
	else 
    {
		if (this->Interactor->GetControlKey()) 
		{
			this->StartSpin();
		}
		else 
		{
			this->StartRotate();
		}
    }
}


//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnMiddleButtonDown() 
{
	if(this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL) {
		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
	}
	if (this->CurrentRenderer == NULL)
    {
		return;
    }
  
	this->GrabFocus(this->EventCallbackCommand);
	this->StartPan();
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnRightButtonDown() 
{
	if(this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL) {
		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
	}
	if (this->CurrentRenderer == NULL)
    {
		return;
    }
  
	this->GrabFocus(this->EventCallbackCommand);
	this->StartDolly();
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnMouseWheelForward() 
{
	if(this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL) {
		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
	}
	if (this->CurrentRenderer == NULL)
    {
		return;
    }
  
	this->GrabFocus(this->EventCallbackCommand);
	this->StartDolly();
	double factor = this->MotionFactor * 0.2 * this->MouseWheelMotionFactor;
	this->Dolly(pow(1.1, factor));
	this->EndDolly();
	this->ReleaseFocus();
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnMouseWheelBackward()
{
	if(this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL) {
		this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
	}
	if (this->CurrentRenderer == NULL)
    {
		return;
    }
  
	this->GrabFocus(this->EventCallbackCommand);
	this->StartDolly();
	double factor = this->MotionFactor * -0.2 * this->MouseWheelMotionFactor;
	this->Dolly(pow(1.1, factor));
	this->EndDolly();
	this->ReleaseFocus();
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
void vtkMAFInteractorStyleTrackballCamera::OnChar() 
{
    vtkRenderWindowInteractor *rwi = this->Interactor;

    switch (rwi->GetKeyCode()) 
    {
    case 'm' :
    case 'M' :
        if (this->AnimState == VTKIS_ANIM_OFF) 
        {
            this->StartAnimate();
        }
        else 
        {
            this->StopAnimate();
        }
        break;

    case 'Q' :
    case 'q' :
    case 'e' :
    case 'E' :
        break;

    case 'f' :      
    case 'F' :
        {
            if(this->CurrentRenderer!=0)
            {
                this->AnimState = VTKIS_ANIM_ON;
                vtkAssemblyPath *path = NULL;
                if (this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL)
                {
                    this->FindPokedRenderer(rwi->GetEventPosition()[0],
                        rwi->GetEventPosition()[1]);
                }
                rwi->GetPicker()->Pick(rwi->GetEventPosition()[0],
                    rwi->GetEventPosition()[1],
                    0.0,
                    this->CurrentRenderer);
                vtkAbstractPropPicker *picker;
                if ((picker=vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker())))
                {
                    path = picker->GetPath();
                }
                if (path != NULL)
                {
                    rwi->FlyTo(this->CurrentRenderer, picker->GetPickPosition());
                }
                this->AnimState = VTKIS_ANIM_OFF;
            }
            else
            {
                vtkWarningMacro(<<"no current renderer on the interactor style.");
            }
        }
        break;

    case 'u' :
    case 'U' :
        rwi->UserCallback();
        break;

    case 'r' :
    case 'R' :
        break;

    case 'w' :
    case 'W' :
        {
            vtkActorCollection *ac;
            vtkActor *anActor, *aPart;
            vtkAssemblyPath *path;
            if (this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL)
            {
                this->FindPokedRenderer(rwi->GetEventPosition()[0],
                    rwi->GetEventPosition()[1]);
            }
            if(this->CurrentRenderer!=0)
            {
                ac = this->CurrentRenderer->GetActors();
                vtkCollectionSimpleIterator ait;
                for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); )
                {
                    for (anActor->InitPathTraversal(); (path=anActor->GetNextPath()); )
                    {
                        aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
                        aPart->GetProperty()->SetRepresentationToWireframe();
                    }
                }
            }
            else
            {
                vtkWarningMacro(<<"no current renderer on the interactor style.");
            }
            rwi->Render();
        }
        break;

    case 's' :
    case 'S' :
        {
            vtkActorCollection *ac;
            vtkActor *anActor, *aPart;
            vtkAssemblyPath *path;
            if (this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL)
            {
                this->FindPokedRenderer(rwi->GetEventPosition()[0],
                    rwi->GetEventPosition()[1]);
            }
            if(this->CurrentRenderer!=0)
            {
                ac = this->CurrentRenderer->GetActors();
                vtkCollectionSimpleIterator ait;
                for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); )
                {
                    for (anActor->InitPathTraversal(); (path=anActor->GetNextPath()); )
                    {
                        aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
                        aPart->GetProperty()->SetRepresentationToSurface();
                    }
                }
            }
            else
            {
                vtkWarningMacro(<<"no current renderer on the interactor style.");
            }
            rwi->Render();
        }
        break;

    case '3' :
        if (rwi->GetRenderWindow()->GetStereoRender()) 
        {
            rwi->GetRenderWindow()->StereoRenderOff();
        }
        else 
        {
            rwi->GetRenderWindow()->StereoRenderOn();
        }
        rwi->Render();
        break;

    case 'p' :
    case 'P' :
        if(this->CurrentRenderer!=0)
        {
            if (this->State == VTKIS_NONE)
            {
                vtkAssemblyPath *path = NULL;
                int *eventPos = rwi->GetEventPosition();
                if (this->UseDefaultRenderer == 0 || this->DefaultRenderer == NULL)
                {
                    this->FindPokedRenderer(eventPos[0],
                        eventPos[1]);
                }
                rwi->StartPickCallback();
                vtkAbstractPropPicker *picker =
                    vtkAbstractPropPicker::SafeDownCast(rwi->GetPicker());
                if ( picker != NULL )
                {
                    picker->Pick(eventPos[0], eventPos[1],
                        0.0, this->CurrentRenderer);
                    path = picker->GetPath();
                }
                if ( path == NULL )
                {
                    this->HighlightProp(NULL);
                    this->PropPicked = 0;
                }
                else
                {
                    this->HighlightProp(path->GetFirstNode()->GetViewProp());
                    this->PropPicked = 1;
                }
                rwi->EndPickCallback();
            }
        }
        else
        {
            vtkWarningMacro(<<"no current renderer on the interactor style.");
        }
        break;
    }
}
