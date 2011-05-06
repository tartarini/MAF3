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
#include <mafPipeVisual.h>

// Foundation Class forwarding list
class vtkPolyDataMapper;
class vtkActor;
class vtkRenderer;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKSurface
 This class represents visual pipe used to render VTK polydata.
 */

class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSurface : public mafResources::mafPipeVisual {
    Q_OBJECT
    Q_PROPERTY(int scalarVisibility READ scalarVisibility WRITE setScalarVisibility)
    Q_PROPERTY(int immediateRendering READ immediateRendering WRITE setImmediateRendering)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
    /// Object constructor;
    mafPipeVisualVTKSurface(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Initialize and create the pipeline.
    /*virtual*/ void createPipe();

    /// Return the flag to activate scalar visibility.
    int scalarVisibility();

    /// Set the flag to activate scalar visibility.
    void setEdgeVisibility(bool edgeVisibility);

    /// Return the flag to activate edge visibility.
    int edgeVisibility();

    /// Return the flag to activate immediate rendering mode.
    int immediateRendering();
    
    /// Pass vtkRenderer to the visual pipe
    /*virtual*/ void setGraphicObject(QObject *graphicObject);


public slots:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Set the visibility of its rendering scene.
    /*virtual*/ void setVisibility(bool visible);


    /// Set the flag to activate scalar visibility.
    void setScalarVisibility(bool scalarVisibility);

    /// Set the flag to activate immediate rendering mode.
    void setImmediateRendering(bool immediateRendering);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSurface();

private:
    vtkPolyDataMapper  *m_Mapper; ///< Class that maps polygonal data.
    vtkRenderer * m_Renderer; ///< Current VTK Renderer. 
    mafCore::mafContainer<vtkActor> m_Actor; ///< Output container.
    int m_ScalarVisibility; ///< Flag to activate scalar visibility.
    int m_ImmediateRendering; ///< Flag to activate immediate rendering mode.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline int mafPipeVisualVTKSurface::scalarVisibility() {
    return m_ScalarVisibility;
}

inline int mafPipeVisualVTKSurface::immediateRendering() {
    return m_ImmediateRendering;
}

} // namespace mafPluginVTK

#endif // MAFVISUALPIPEVTKSURFACE_H
