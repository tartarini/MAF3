/*
 *  mafInteractorVTKCamera.h
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 7/7/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACTORVTKCAMERA_H
#define MAFINTERACTORVTKCAMERA_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafInteractor.h>

// Foundation Class forwarding list
class vtkRenderWindowInteractor;
class vtkInteractorStyleTrackballCamera;

namespace mafPluginVTK {

/**
Class name: mafInteractorVTKCamera
This class represent an interactor implementing a picking operation.
*/

class MAFPLUGINVTKSHARED_EXPORT mafInteractorVTKCamera : public mafResources::mafInteractor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInteractor);

public:
    /// Object constructor.
    mafInteractorVTKCamera(const QString code_location = "");

public Q_SLOTS:
    /// Called when the any mouse button is pressed
    /*virtual*/ void mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

    /// Called when the any mouse button is released
    /*virtual*/ void mouseRelease(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

    /// Called when mouse is moved
    /*virtual*/ void mouseMove(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

protected:
    /// Object destructor.
    /* virtual */~mafInteractorVTKCamera();

private:
    vtkRenderWindowInteractor *m_RenderWindowInteractor; ///< VTK Interactor
    vtkInteractorStyleTrackballCamera *m_CurrentVTKInteractor; ///< VTK Interactor Style
};


} // namespace mafPluginVTK

#endif // MAFINTERACTORVTKCAMERA_H
