/*
 *  mafPipeVisualVTKSurface.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPEVTKSURFACE_H
#define MAFVISUALPIPEVTKSURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"

// Foundation Class forwarding list
class vtkPolyDataMapper;
class vtkActor;
class vtkRenderer;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKSurface
 This class represents visual pipe used to render VTK polydata.
 */

class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSurface : public mafPipeVisualVTK {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKSurface(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSurface();
    
private:
    vtkPolyDataMapper  *m_Mapper; ///< Class that maps polygonal data.
    vtkRenderer * m_Renderer; ///< Current VTK Renderer. 
    bool m_ScalarVisibility; ///< Flag to activate scalar visibility.
    bool m_ImmediateRendering; ///< Flag to activate immediate rendering mode.

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);

    // Open color dialog
    void on_colorButton_released();
};

} // namespace mafPluginVTK

#endif // MAFVISUALPIPEVTKSURFACE_H
