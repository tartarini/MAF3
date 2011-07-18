/*
 *  mafInteractorVTKTransform.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 7/7/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <QMouseEvent>

#include "mafInteractorVTKTransform.h"
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

mafInteractorVTKTransform::mafInteractorVTKTransform(const QString code_location) : mafInteractor(code_location), m_PositionInitialized(false) {
}

mafInteractorVTKTransform::~mafInteractorVTKTransform(){
}

void mafInteractorVTKTransform::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    QMouseEvent *me = (QMouseEvent *) e;
    if(me->button() != Qt::LeftButton) {
        return;
    }
    
    mafVTKWidget *w = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if(w == NULL) {
        return;
    }
    
    m_Renderer = w->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    
    Translate(me->pos());
    
    m_PositionInitialized = true;
}


void mafInteractorVTKTransform::mouseRelease(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    m_Renderer = NULL;
    m_PositionInitialized = false;
}

void mafInteractorVTKTransform::mouseMove(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    /// if LeftButton is pressed
    if(m_Renderer == NULL) {
        return;
    }
    
    mafResources::mafVME *vme = qobject_cast<mafResources::mafVME*>(obj);
    if(vme == NULL) {
        return;
    }
    
    QMouseEvent *me = (QMouseEvent *) e;
    Translate(me->pos());
    
    //Check if ctrl is pressed
    if((modifiers&(1<<MAF_CTRL_KEY))!=0) {
        
    } else {
        //this->internalUpdate();
    }

    
}

void mafInteractorVTKTransform::Translate(QPoint point2D) {    
    //    vtkCamera *camera = renderer->GetActiveCamera();
//    double viewPlaneNormal[3];
//    double viewUp[3];
//    camera->GetViewPlaneNormal(viewPlaneNormal);
//    camera->GetViewUp(viewUp);
//    
    double coordinates[3]; 
    coordinates[0] = point2D.x(); 
    coordinates[1] = point2D.y(); 
    coordinates[2] = 0; 
    
    m_Renderer->SetDisplayPoint(coordinates); 
    m_Renderer->DisplayToView(); 
    m_Renderer->GetViewPoint(coordinates); 
    m_Renderer->ViewToWorld(); 
    double *wNewPoint = m_Renderer->GetWorldPoint(); 
    
    // apply transform to vme
    if(m_PositionInitialized) {
        double pos[3];
        m_VME->dataSetCollection()->position(pos);
        
        pos[0] += (wNewPoint[0] - m_LastPosition3D[0]) /50. ; 
        pos[1] += (wNewPoint[1] - m_LastPosition3D[1]) /50. ;
        pos[2] += (wNewPoint[2] - m_LastPosition3D[2]) /50. ;

        m_VME->dataSetCollection()->setPosition(pos);
        qDebug() << pos[0]  << " " << pos[1] << " " << pos[2];
    }
    
    // memorize old position
    m_LastPosition3D[0] = wNewPoint[0];
    m_LastPosition3D[1] = wNewPoint[1];
    m_LastPosition3D[2] = wNewPoint[2];
}
