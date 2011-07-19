/*
 *  mafInteractorVTKCamera.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 7/7/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <QMouseEvent>

#include "mafInteractorVTKCamera.h"
#include "mafVTKWidget.h"
#include <mafMatrix.h>
#include <mafVME.h>
#include <mafDataSetCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

using namespace mafResources;
using namespace mafPluginVTK;

mafInteractorVTKCamera::mafInteractorVTKCamera(const QString code_location) : mafInteractor(code_location) {
}

mafInteractorVTKCamera::~mafInteractorVTKCamera(){
}

void mafInteractorVTKCamera::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
}


void mafInteractorVTKCamera::mouseRelease(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
}

void mafInteractorVTKCamera::mouseMove(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
}
