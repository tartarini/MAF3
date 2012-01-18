/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMAFImplicitPlaneRepresentation.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMAFImplicitPlaneRepresentation.h"

#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkAssemblyPath.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(vtkMAFImplicitPlaneRepresentation);

//----------------------------------------------------------------------------
vtkMAFImplicitPlaneRepresentation::vtkMAFImplicitPlaneRepresentation()
{
  this->RotationEnabled = 1;
  this->DrawOutlineBox = 1;

  this->Picker->AddPickList(this->EdgesActor);
  this->EdgesActor->SetProperty(this->EdgesProperty);
}

//----------------------------------------------------------------------------
vtkMAFImplicitPlaneRepresentation::~vtkMAFImplicitPlaneRepresentation()
{  
}

//----------------------------------------------------------------------------
int vtkMAFImplicitPlaneRepresentation::ComputeInteractionState(int X, int Y, int vtkNotUsed(modify))
{
    // See if anything has been selected
    vtkAssemblyPath *path;
    this->Picker->Pick(X,Y,0.0,this->Renderer);
    path = this->Picker->GetPath();

    if ( path == NULL ) //not picking this widget
    {
        this->SetRepresentationState(vtkImplicitPlaneRepresentation::Outside);
        this->InteractionState = vtkImplicitPlaneRepresentation::Outside;
        return this->InteractionState;
    }

    // Something picked, continue
    this->ValidPick = 1;

    // Depending on the interaction state (set by the widget) we modify
    // this state based on what is picked.
    if ( this->InteractionState == vtkImplicitPlaneRepresentation::Moving )
    {
        vtkProp *prop = path->GetFirstNode()->GetViewProp();
        if ( prop == this->ConeActor || prop == this->LineActor ||
            prop == this->ConeActor2 || prop == this->LineActor2 )
        {
            this->InteractionState = vtkImplicitPlaneRepresentation::Rotating;
            this->SetRepresentationState(vtkImplicitPlaneRepresentation::Rotating);
        }
        else if ( prop == this->CutActor || prop == this->EdgesActor )
        {
            this->InteractionState = vtkImplicitPlaneRepresentation::Pushing;
            this->SetRepresentationState(vtkImplicitPlaneRepresentation::Pushing);
        }
        else if ( prop == this->SphereActor )
        {
            this->InteractionState = vtkImplicitPlaneRepresentation::MovingOrigin;
            this->SetRepresentationState(vtkImplicitPlaneRepresentation::MovingOrigin);
        }
        else
        {
            if ( this->OutlineTranslation )
            {
                this->InteractionState = vtkImplicitPlaneRepresentation::MovingOutline;
                this->SetRepresentationState(vtkImplicitPlaneRepresentation::MovingOutline);
            }
            else
            {
                this->InteractionState = vtkImplicitPlaneRepresentation::Outside;
                this->SetRepresentationState(vtkImplicitPlaneRepresentation::Outside);
            }
        }
    }

    else if ( this->InteractionState == vtkImplicitPlaneRepresentation::MovingPlane )
    {
        return this->InteractionState;
    }

    else if ( this->InteractionState == vtkImplicitPlaneRepresentation::Scaling )
    {
        return this->InteractionState;
    }

    else
    {
        this->InteractionState = vtkImplicitPlaneRepresentation::Outside;
    }

    return this->InteractionState;
}

//----------------------------------------------------------------------------
int vtkMAFImplicitPlaneRepresentation::RenderOpaqueGeometry(vtkViewport *v)
{
	int count = 0;
	this->BuildRepresentation();
    if ( this->DrawOutlineBox)
    {
	    count += this->OutlineActor->RenderOpaqueGeometry(v);
    }
	count += this->EdgesActor->RenderOpaqueGeometry(v);
	if ( this->RotationEnabled )
	{
		count += this->ConeActor->RenderOpaqueGeometry(v);
		count += this->LineActor->RenderOpaqueGeometry(v);
		count += this->ConeActor2->RenderOpaqueGeometry(v);
		count += this->LineActor2->RenderOpaqueGeometry(v);
	}
	count += this->SphereActor->RenderOpaqueGeometry(v);
	if ( this->DrawPlane )
	{
		count += this->CutActor->RenderOpaqueGeometry(v);
	}

	return count;
}

//-----------------------------------------------------------------------------
int vtkMAFImplicitPlaneRepresentation::RenderTranslucentPolygonalGeometry(vtkViewport *v)
{
	int count = 0;
	this->BuildRepresentation();
    if ( this->DrawOutlineBox)
    {
        count += this->OutlineActor->RenderTranslucentPolygonalGeometry(v);
    }
	count += this->EdgesActor->RenderTranslucentPolygonalGeometry(v);
	if ( this->RotationEnabled )
	{
		count += this->ConeActor->RenderTranslucentPolygonalGeometry(v);
		count += this->LineActor->RenderTranslucentPolygonalGeometry(v);
		count += this->ConeActor2->RenderTranslucentPolygonalGeometry(v);
		count += this->LineActor2->RenderTranslucentPolygonalGeometry(v);
	}
	count += this->SphereActor->RenderTranslucentPolygonalGeometry(v);
	if ( this->DrawPlane )
	{
		count += this->CutActor->RenderTranslucentPolygonalGeometry(v);
	}

	return count;
}

//-----------------------------------------------------------------------------
int vtkMAFImplicitPlaneRepresentation::HasTranslucentPolygonalGeometry()
{
	int result=0;
    if ( this->DrawOutlineBox)
    {
        result |= this->OutlineActor->HasTranslucentPolygonalGeometry();
    }
	result |= this->EdgesActor->HasTranslucentPolygonalGeometry();

	if ( this->RotationEnabled )
	{
		result |= this->ConeActor->HasTranslucentPolygonalGeometry();
		result |= this->LineActor->HasTranslucentPolygonalGeometry();
		result |= this->ConeActor2->HasTranslucentPolygonalGeometry();
		result |= this->LineActor2->HasTranslucentPolygonalGeometry();
	}
	result |= this->SphereActor->HasTranslucentPolygonalGeometry();
	if ( this->DrawPlane )
	{
		result |= this->CutActor->HasTranslucentPolygonalGeometry();
	}

	return result;
}

//----------------------------------------------------------------------------
void vtkMAFImplicitPlaneRepresentation::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);

	os << indent << "Rotation Enabled: " << (this->RotationEnabled ? "On" : "Off") << "\n";

	// this->InteractionState is printed in superclass
	// this is commented to avoid PrintSelf errors
}

//----------------------------------------------------------------------------
void vtkMAFImplicitPlaneRepresentation::SetRotationEnabled(int enable)
{
	if ( enable == this->RotationEnabled )
	{
		return;
	}

    this->ConeActor->SetPickable(enable);
    this->LineActor->SetPickable(enable);
    this->ConeActor2->SetPickable(enable);
    this->LineActor2->SetPickable(enable);

    this->Modified();
	this->RotationEnabled = enable;
	this->BuildRepresentation();
}

//----------------------------------------------------------------------------
void vtkMAFImplicitPlaneRepresentation::SetDrawOutlineBox(int box)
{
    if ( box == this->DrawOutlineBox )
    {
        return;
    }

    this->Modified();
    this->DrawOutlineBox = box;
    this->BuildRepresentation();
}
