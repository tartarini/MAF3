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
#include <mafSceneNode.h>

// Class forwarding list
class mafEventBridgeVTK;
class vtkRenderer;

namespace mafPluginVTK {

class mafVTKWidget;

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

    /*virtual*/ void removeSceneNode(mafResources::mafSceneNode *node);

    /*virtual*/ void showSceneNode(mafResources::mafSceneNode *node, bool show = true, const mafString visualPipeType = "");

private:
    mafCore::mafContainer<mafPluginVTK::mafVTKWidget> m_Widget; ///< VTK Widget on which will be rendered the scene.
    vtkRenderer *m_Renderer; ///< VTK render.
};

} //namespace mafPluginVTK

#endif // MAFVIEWVTK_H
