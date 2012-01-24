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
#include "mafPipeVisualVTK.h"
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
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSliceVolume : public mafPipeVisualVTK {
    Q_OBJECT
    Q_PROPERTY(QString originX READ originX)
    Q_PROPERTY(QString originY READ originY)
    Q_PROPERTY(QString originZ READ originZ)
    Q_PROPERTY(QString normalX READ normalX WRITE setNormalX)
    Q_PROPERTY(QString normalY READ normalY WRITE setNormalY)
    Q_PROPERTY(QString normalZ READ normalZ WRITE setNormalZ)
    Q_PROPERTY(QString sliceAxes READ sliceAxes WRITE setSliceAxes)
    
    Q_PROPERTY(int positionSlider_minimum READ minimumPositionSlider)
    Q_PROPERTY(int positionSlider_maximum READ maximumPositionSlider)
    Q_PROPERTY(int positionSlider_value READ valuePositionSlider)
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKSliceVolume(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Set the origin of the slice
    void setSlice(double *origin);

    /// Set the normal of the slice
    void setNormal(double *normal);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Set originX value from text box.
    void on_originX_textEdited(QString stringValue);

    /// Set originY value from text box.
    void on_originY_textEdited(QString stringValue);

    /// Set originZ value from text box.
    void on_originZ_textEdited(QString stringValue);

    /// Get originX value;
    QString originX();

    /// Get originY value;
    QString originY();

    /// Get originZ value;
    QString originZ();

    /// Set normalX value from text box.
    void on_normalX_textEdited(QString stringValue);

    /// Set normalY value from text box.
    void on_normalY_textEdited(QString stringValue);

    /// Set normalZ value from text box.
    void on_normalZ_textEdited(QString stringValue);

    /// Get normalX value;
    QString normalX();

    /// Set normalX value;
    void setNormalX(QString value);

    /// Get normalY value;
    QString normalY();

    /// Set normalY value;
    void setNormalY(QString value);

    /// Get normalZ value;
    QString normalZ();

    /// Set normalZ value;
    void setNormalZ(QString value);

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
    double m_Origin[3]; ///< Origin of the slicing plane.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafPipeVisualVTKSliceVolume::on_originX_textEdited(QString stringValue) {
    m_Origin[0] = stringValue.toDouble();
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_originY_textEdited(QString stringValue) {
    m_Origin[1] = stringValue.toDouble();
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_originZ_textEdited(QString stringValue) {
    m_Origin[2] = stringValue.toDouble();
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_normalX_textEdited(QString stringValue) {
    setNormalX(stringValue);
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_normalY_textEdited(QString stringValue) {
    setNormalY(stringValue);
    setModified();
}

inline void mafPipeVisualVTKSliceVolume::on_normalZ_textEdited(QString stringValue) {
    setNormalZ(stringValue);
    setModified();
}

inline QString mafPipeVisualVTKSliceVolume::normalX() {
    return QString::number(m_SlicerPipe->normal()[0]);
}

inline void mafPipeVisualVTKSliceVolume::setNormalX(QString stringValue) {
    double *n = m_SlicerPipe->normal();
    n[0] = stringValue.toDouble();
    m_SlicerPipe->setNormal(n);
}

inline QString mafPipeVisualVTKSliceVolume::normalY() {
    return QString::number(m_SlicerPipe->normal()[1]);
}

inline void mafPipeVisualVTKSliceVolume::setNormalY(QString stringValue) {
    double *n = m_SlicerPipe->normal();
    n[1] = stringValue.toDouble();
    m_SlicerPipe->setNormal(n);
}

inline QString mafPipeVisualVTKSliceVolume::normalZ() {
    return QString::number(m_SlicerPipe->normal()[2]);
}

inline void mafPipeVisualVTKSliceVolume::setNormalZ(QString stringValue) {
    double *n = m_SlicerPipe->normal();
    n[2] = stringValue.toDouble();
    m_SlicerPipe->setNormal(n);
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
