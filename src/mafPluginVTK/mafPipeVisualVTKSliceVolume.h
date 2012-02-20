/*
 *  mafPipeVisualVTKSliceVolume.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 09/11/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEVISUALVTKSLICEVOLUME_H
#define MAFPIPEVISUALVTKSLICEVOLUME_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTKSlice.h"
#include "mafPipeDataSliceVolume.h"

#include <vtkSmartPointer.h>

// Foundation Class forwarding list
class vtkPolyDataMapper;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafPipeVisualVTKSliceVolume
 This visual pipe allow to extract a contour from a Volume data given a cut origin.
*/
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSliceVolume : public mafPipeVisualVTKSlice {
    Q_OBJECT
    Q_PROPERTY(QString sliceAxes READ sliceAxes WRITE setSliceAxes)
    
    Q_PROPERTY(int positionSlider_minimum READ minimumPositionSlider)
    Q_PROPERTY(int positionSlider_maximum READ maximumPositionSlider)
    Q_PROPERTY(int positionSlider_value READ valuePositionSlider)
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTKSlice);

public:
    /// Object constructor;
    mafPipeVisualVTKSliceVolume(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Set originX value from text box.
    void on_originX_textEdited(QString stringValue);

    /// Set originY value from text box.
    void on_originY_textEdited(QString stringValue);

    /// Set originZ value from text box.
    void on_originZ_textEdited(QString stringValue);

    /// Set normalX value from text box.
    void on_normalX_textEdited(QString stringValue);

    /// Set normalY value from text box.
    void on_normalY_textEdited(QString stringValue);

    /// Set normalZ value from text box.
    void on_normalZ_textEdited(QString stringValue);

    /// Set the slice axes normal to the slicing plane.
    QString sliceAxes() const;

    /// Assign the slice axes normal to the slicing plane.
    void setSliceAxes(QString axes);
    
    /// retrieve minimum value in slider range.
    int minimumPositionSlider() const;
    
    /// retrieve maximum value in slider range.
    int maximumPositionSlider() const;

    /// retrieve current value in slider range.
    int valuePositionSlider() const;

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSliceVolume();

private:
    mafPipeDataSliceVolume *m_SlicerPipe;    ///< Slicer data pipe.
    vtkSmartPointer<vtkPolyDataMapper> m_Mapper; ///< Data mapper.
    double m_Range[2]; ///position range.
    double m_CurrentValue; // current position.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafPipeVisualVTKSliceVolume::on_originX_textEdited(QString stringValue) {
    m_Origin->setX(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_originY_textEdited(QString stringValue) {
    m_Origin->setY(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_originZ_textEdited(QString stringValue) {
    m_Origin->setZ(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_normalX_textEdited(QString stringValue) {
    m_Normal->setX(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_normalY_textEdited(QString stringValue) {
    m_Normal->setY(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_normalZ_textEdited(QString stringValue) {
    m_Normal->setZ(stringValue.toDouble());
    setModified();
}

inline QString mafPipeVisualVTKSliceVolume::sliceAxes() const {
    return QString::number(m_SlicerPipe->planeNormalAxes());
}

inline void mafPipeVisualVTKSliceVolume::setSliceAxes(QString axes) {
    mafResources::mafPlaneNormal n = (mafResources::mafPlaneNormal)axes.toInt();
    m_SlicerPipe->setPlaneNormalAxes(n);
}

inline int mafPipeVisualVTKSliceVolume::minimumPositionSlider() const {
    return m_Range[0];
}

inline int mafPipeVisualVTKSliceVolume::maximumPositionSlider() const {
    return m_Range[1];
}

inline int mafPipeVisualVTKSliceVolume::valuePositionSlider() const {
    return m_CurrentValue;
}

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICEVOLUME_H
