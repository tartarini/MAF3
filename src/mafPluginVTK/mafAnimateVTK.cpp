/*
 *  mafAnimateVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 21/02/12.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafAnimateVTK.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkMath.h>

using namespace mafCore;
using namespace mafPluginVTK;


mafAnimateVTK::mafAnimateVTK(const QString code_location) : mafCore::mafObjectBase(code_location) {
    setObjectName("mafAnimateVTK");
}

void mafAnimateVTK::flyTo(mafVTKWidget *widget, double bounds[6]) {
    REQUIRE(widget);
    vtkRenderer *renderer = widget->renderer("tool");
    double center[3]; 
    center[0] = (bounds[0] + bounds[1]) / 2;
    center[1] = (bounds[2] + bounds[3]) / 2;
    center[2] = (bounds[4] + bounds[5]) / 2;

    vtkCamera *camera = renderer->GetActiveCamera();

    double fly0[10]; // from
    double fly1[10]; // to
    double fly [10]; // interpolated position
    double distance;
    double vn[3], *vup;
    camera->GetViewPlaneNormal(vn);

    fly0[0] = camera->GetFocalPoint()[0];
    fly0[1] = camera->GetFocalPoint()[1];
    fly0[2] = camera->GetFocalPoint()[2];
    fly0[3] = camera->GetPosition()[0];
    fly0[4] = camera->GetPosition()[1];
    fly0[5] = camera->GetPosition()[2];
    fly0[6] = camera->GetViewUp()[0];
    fly0[7] = camera->GetViewUp()[1];
    fly0[8] = camera->GetViewUp()[2];
    fly0[9] = camera->GetParallelScale();

    ///new camera parameters found using code of vtkRenderer::ResetCamera(double bounds[6])
    double w1 = bounds[1] - bounds[0];
    double w2 = bounds[3] - bounds[2];
    double w3 = bounds[5] - bounds[4];
    w1 *= w1;
    w2 *= w2;
    w3 *= w3;
    double radius = w1 + w2 + w3;

    radius = (radius==0)?(1.0):(radius);
    radius = sqrt(radius)*0.5;

    double angle=vtkMath::RadiansFromDegrees(camera->GetViewAngle());
    double parallelScale=radius;

    renderer->ComputeAspect();
    double aspect[2];
    renderer->GetAspect(aspect);

    if(aspect[0]>=1.0) { 
        // horizontal window, deal with vertical angle|scale
        if(camera->GetUseHorizontalViewAngle()) {
            angle=2.0*atan(tan(angle*0.5)/aspect[0]);
        }
    } else { 
        // vertical window, deal with horizontal angle|scale
        if(!camera->GetUseHorizontalViewAngle()) {
            angle=2.0*atan(tan(angle*0.5)*aspect[0]);
        }

        parallelScale=parallelScale/aspect[0];
    }

    distance =radius/sin(angle*0.5);
    vup = camera->GetViewUp();

    fly1[0] = center[0];
    fly1[1] = center[1];
    fly1[2] = center[2];
    fly1[3] = center[0]+distance*vn[0];
    fly1[4] = center[1]+distance*vn[1];
    fly1[5] = center[2]+distance*vn[2];
    fly1[6] = -vup[2];
    fly1[7] = vup[0];
    fly1[8] = vup[1];
    fly1[9] = fly0[9];

    int numSteps = 120;
    int rate = 15;
    double pi = vtkMath::Pi();

    for (int i = 0; i <= numSteps; i++) {
        double t  = ( i * 1.0 ) / numSteps;
        double t2 = 0.5 + 0.5 * sin( t*pi - pi/2 );

        for(int j = 0; j < 10 ; j ++ ) {
            fly[j] = (1-t2) * fly0[j] + t2 * fly1[j];
        }

        camera->SetFocalPoint(fly[0],fly[1],fly[2]);
        camera->SetPosition(fly[3],fly[4],fly[5]);
        camera->SetViewUp(fly[6],fly[7],fly[8]);
        camera->SetParallelScale(fly[9]);
        renderer->ResetCameraClippingRange();
        if (widget->GetRenderWindow()) {
            widget->GetRenderWindow()->Render();
        }
    }
    renderer->ResetCameraClippingRange();
}

mafAnimateVTK::~mafAnimateVTK() {

}
