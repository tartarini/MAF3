/*
 *  mafPipeVisualVTKSlice.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 17/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEVISUALVTKSLICE_H
#define MAFPIPEVISUALVTKSLICE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"
#include <mafPoint.h>

#include <vtkSmartPointer.h>

// Foundation Class forwarding list
class vtkPolyDataMapper;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafPipeVisualVTKSlice
 This class is the base for all the visual pipe to visually represent sliced objects.
*/
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSlice : public mafPipeVisualVTK {
    Q_OBJECT
    Q_PROPERTY(mafResources::mafPointPointer origin READ origin)
    Q_PROPERTY(mafResources::mafPointPointer normal READ normal)
    Q_PROPERTY(QString positionValue READ positionValue WRITE setPositionValue)
    Q_PROPERTY(int positionValueSlider_minimum READ minimum)
    Q_PROPERTY(int positionValueSlider_maximum READ maximum)
    Q_PROPERTY(int positionValueSlider_value READ value)
    
    
    
//     Q_PROPERTY(QString sliceAxes READ sliceAxes WRITE setSliceAxes)
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKSlice(const QString code_location = "");

    /// Set the origin of the slice
    void setSlice(mafResources::mafPointPointer o);

    /// Set plane normal vector;
    void setNormal(mafResources::mafPointPointer val);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);

    /// Return the pointer to the origin of the slicing plane.
    mafResources::mafPointPointer origin();

    /// Return the pointer to the normal of the slicing plane.
    mafResources::mafPointPointer normal();

    /// Set contour value from text box.
    void on_positionValue_textEdited(QString stringValue);

    /// Update pipe when when editing is finished.
    void on_positionValue_editingFinished();

    /// Set position value from slider.
    void on_positionValueSlider_sliderMoved(int value);


    /// Get contour value;
    QString positionValue();

    /// Set contour value;
    void setPositionValue(QString value);

    /// Set minimum of slider.
    int minimum();

    /// Set maximum of slider.
    int maximum();

    /// Set Slider postion;
    int value();

    /// Set the slice axes normal to the slicing plane.
//     QString sliceAxes() const;

    /// Assign the slice axes normal to the slicing plane.
//     void setSliceAxes(QString axes);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSlice();

    mafResources::mafPoint *m_Normal; ///< Normal of the slicing plane.
    mafResources::mafPoint *m_Origin; ///< Origin of the slicing plane.
    int m_PositionValue; ///< Position of the slicing plane.
    double m_Range[2]; ///Contour range.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline int mafPipeVisualVTKSlice::minimum() {
    return m_Range[0];
}    

inline int mafPipeVisualVTKSlice::maximum() {
    return m_Range[1];
}  

inline int mafPipeVisualVTKSlice::value() {
    return m_PositionValue;
}  

// inline QString mafPipeVisualVTKSlice::sliceAxes() const {
//     return QString::number(m_SlicerPipe->planeNormalAxes());
// }

// inline void mafPipeVisualVTKSlice::setSliceAxes(QString axes) {
//     mafResources::mafPlaneNormal n = (mafResources::mafPlaneNormal)axes.toInt();
//     m_SlicerPipe->setPlaneNormalAxes(n);
// }

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICEVOLUME_H
