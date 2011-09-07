/*
 *  mafPipeVisualVTKIsoSurface.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/05/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPEVTKISOSURFACE_H
#define MAFVISUALPIPEVTKISOSURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"

// Foundation Class forwarding list
class vtkPolyDataMapper;
class vtkActor;
class vtkRenderer;
class vtkContourFilter;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKIsoSurface
 This visual pipe allow to extract a surface from a volume data given a 
 threshold value. The value is extracted according to the scalar values present 
 into the volume data. The iso-surface is extracted in real time*/

class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKIsoSurface : public mafPipeVisualVTK {
    Q_OBJECT
    Q_PROPERTY(QString contourValue READ contourValue WRITE setContourValue)
    Q_PROPERTY(int contourValueSlider_minimum READ minimum)
    Q_PROPERTY(int contourValueSlider_maximum READ maximum)
    Q_PROPERTY(int contourValueSlider_sliderPosition READ sliderPosition)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);
    

public:
    /// Object constructor;
    mafPipeVisualVTKIsoSurface(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Set contour value from text box.
    void on_contourValue_textEdited(QString stringValue);

    /// Update pipe when when editing  is finished.
    void on_contourValue_editingFinished();

    /// Set contour value from slider.
    void on_contourValueSlider_valueChanged(int value);

    /// Update pipe when slider is released.
    void on_contourValueSlider_sliderReleased();

    /// Get contour value;
    QString contourValue();

    /// Set contour value;
    void setContourValue(QString value);

    /// Set minimum of slider.
    int minimum();

    /// Set maximum of slider.
    int maximum();

    /// Set Slider postion;
    int sliderPosition();

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKIsoSurface();

private:
    vtkPolyDataMapper  *m_Mapper; ///< Class that maps polygonal data.
    vtkRenderer * m_Renderer; ///< Current VTK Renderer. 
    vtkContourFilter* m_ContourFilter; ///VTKContourFilter.
    bool m_ScalarVisibility; ///< Flag to activate scalar visibility.
    bool m_ImmediateRendering; ///< Flag to activate immediate rendering mode.
    int m_ContourValue; ///< Value to be applied to the contour fileter.
    double m_Range[2]; ///Contour range.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline int mafPipeVisualVTKIsoSurface::minimum() {
    return m_Range[0];
}    

inline int mafPipeVisualVTKIsoSurface::maximum() {
    return m_Range[1];
}  

inline int mafPipeVisualVTKIsoSurface::sliderPosition() {
    return m_ContourValue;
}   

} // namespace mafPluginVTK

#endif // MAFVISUALPIPEVTKISOSURFACE_H
