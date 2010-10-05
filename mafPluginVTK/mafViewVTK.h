/*
 *  mafViewVTK.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWVTK_H
#define MAFVIEWVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafView.h>
#include <mafContainer.h>
#include <mafContainerInterface.h>
#include <mafVME.h>

// Class forwarding list
class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;

namespace mafPluginVTK {

/**
 Class name: mafViewVTK
 This is the VTK MAF3 views.
 */

class MAFPLUGINVTKSHARED_EXPORT mafViewVTK : public mafResources::mafView {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafView);

public:
    /// Object constructor.
    mafViewVTK(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafViewVTK();

    /*virtual*/ void create();

    /*virtual*/ void removeVME(mafResources::mafVME *vme);

    /*virtual*/ void showVME(mafResources::mafVME *vme, bool show = true, const mafString visualPipeType = "");

private:
    mafCore::mafContainer<vtkRenderWindow> *m_RenWin;  ///< VTK rendering window.
    vtkRenderer *m_Renderer; ///< VTK render.
    vtkRenderWindowInteractor *m_Iren; ///< VTK renderer interactor.

};

} //namespace mafPluginVTK

#endif // MAFVIEWVTK_H
