/*
 *  mafPipeVisualVTKMIPVolume.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/05/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPEVTKMIPVOLUME_H
#define MAFVISUALPIPEVTKMIPVOLUME_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"

// Foundation Class forwarding list
class vtkVolumeRayCastMapper;
class vtkPiecewiseFunction;
class vtkVolume;
class vtkRenderer;
class vtkDataSet;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKMIPVolume
 This visual pipe use MIP volume rendering for visualizing vtkImageData or Structured Points.
 */
                                
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKMIPVolume : public mafPipeVisualVTK {
    Q_OBJECT
    //Q_PROPERTY(type name READ function)

    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKMIPVolume(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

public slots:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKMIPVolume();

private:
    /// update piecewise, color transfer functions.
    void updateFunctions(vtkDataSet *dataset);
    
    vtkPiecewiseFunction* m_PieceWiseTransferFunction;
    vtkVolumeRayCastMapper  *m_Mapper; ///< Class that maps polygonal data.
    vtkRenderer * m_Renderer; ///< Current VTK Renderer. 
    bool m_ImmediateRendering; ///< Flag to activate immediate rendering mode.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

} // namespace mafPluginVTK

#endif // MAFVISUALPIPEVTKMIPVOLUME_H
