/*
 *  mafViewVTK.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWVTK_H
#define MAFVIEWVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafView.h>

// Class forwarding list
class vtkRenderer;

namespace mafPluginVTK {


typedef enum {
    mafCameraDirectionX = 0,
    mafCameraDirectionY,
    mafCameraDirectionZ,
    mafCameraDirectionXNegative,
    mafCameraDirectionYNegative,
    mafCameraDirectionZNegative
} mafCameraDirection;

class mafVTKWidget;

/**
 Class name: mafViewVTK
 This is the VTK MAF3 views.
 */
class MAFPLUGINVTKSHARED_EXPORT mafViewVTK : public mafResources::mafView {
    Q_OBJECT
    Q_PROPERTY(bool cameraParallel READ cameraParallel WRITE setCameraParallel)
    Q_PROPERTY(int cameraAxes READ cameraAxes WRITE setCameraAxes)

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafView);

public Q_SLOTS:
    /// Update view.
    /*virtual*/ void updateView();

    /// Reset the visualization to show visible objects
    /*virtual*/ void resetVisualization(double *bounds = NULL);

public:
    /// Object constructor.
    mafViewVTK(const QString code_location = "");

    /// Crete view.
    /*virtual*/ bool initialize();

    /// Remove scene node passed as argument.
    /*virtual*/ void removeSceneNode(mafResources::mafSceneNode *node);

    /// Called to show/hide the node.
    /*virtual*/ void showSceneNode(mafResources::mafSceneNode *node, bool show = true);

    /// Return the camera parallel flag.
    bool cameraParallel() const;

    /// Allows to assign the camera parallel flag.
    void setCameraParallel(bool parallel = true);

    /// Return the camera axes direction.
    int cameraAxes() const;

    /// Set the camera direction.
    void setCameraAxes(int axes);

protected:
    /// Object destructor.
    /* virtual */ ~mafViewVTK();

private:
    vtkRenderer *m_Renderer; ///< VTK render.
    bool m_CameraParallel; ///< Flag that store the information on camera type: true means Parallel otherwise Perspective.
    mafCameraDirection m_CameraAxesDirection; ///< Direction in which the camera is looking to.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline bool mafViewVTK::cameraParallel() const {
    return m_CameraParallel;
}

inline void mafViewVTK::setCameraParallel(bool parallel /* = true */) {
    m_CameraParallel = parallel;
}

inline int mafViewVTK::cameraAxes() const {
    return (int)m_CameraAxesDirection;
}

} //namespace mafPluginVTK

#endif // MAFVIEWVTK_H
