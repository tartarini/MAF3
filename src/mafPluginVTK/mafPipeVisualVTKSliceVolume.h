/*
 *  mafPipeVisualVTKSliceVolume.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 09/11/11.
 *  Copyright 2011 B3C. All rights reserved.
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
    Q_PROPERTY(QString originX READ originX WRITE setOriginX)
    Q_PROPERTY(QString originY READ originY WRITE setOriginY)
    Q_PROPERTY(QString originZ READ originZ WRITE setOriginZ)
    Q_PROPERTY(QString normalX READ normalX WRITE setNormalX)
    Q_PROPERTY(QString normalY READ normalY WRITE setNormalY)
    Q_PROPERTY(QString normalZ READ normalZ WRITE setNormalZ)
    Q_PROPERTY(QString thickness READ thickness WRITE setThickness)

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

    /// Set originX value;
    void setOriginX(QString stringValue);

    /// Get originY value;
    QString originY();

    /// Set originY value;
    void setOriginY(QString stringValue);

    /// Get originZ value;
    QString originZ();

    /// Set originZ value;
    void setOriginZ(QString stringValue);

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

    /// Get thickness value;
    QString thickness();

    /// Set thickness value;
    void setThickness(QString value);

    /// Set thickness value from text box.
    void on_thickness_textEdited(QString stringValue);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSliceVolume();

private:
    double m_Thickness; ///< Contour thickness.

    mafPipeDataSliceVolume *m_SlicerPipe;    ///< Slicer data pipe.
    vtkSmartPointer<vtkPolyDataMapper> m_Mapper; ///< Data mapper.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QString mafPipeVisualVTKSliceVolume::originX() {
    return QString::number(m_SlicerPipe->sliceOrigin()[0]);
}

inline void mafPipeVisualVTKSliceVolume::setOriginX(QString stringValue) {
    double *o = m_SlicerPipe->sliceOrigin();
    o[0] = stringValue.toDouble();
    m_SlicerPipe->setSliceOrigin(o);
}

inline void mafPipeVisualVTKSliceVolume::on_originX_textEdited(QString stringValue) {
    setOriginX(stringValue);
}

inline QString mafPipeVisualVTKSliceVolume::originY() {
    return QString::number(m_SlicerPipe->sliceOrigin()[1]);
}

inline void mafPipeVisualVTKSliceVolume::setOriginY(QString stringValue) {
    double *o = m_SlicerPipe->sliceOrigin();
    o[1] = stringValue.toDouble();
    m_SlicerPipe->setSliceOrigin(o);
}

inline void mafPipeVisualVTKSliceVolume::on_originY_textEdited(QString stringValue) {
    setOriginY(stringValue);
}

inline QString mafPipeVisualVTKSliceVolume::originZ() {
    return QString::number(m_SlicerPipe->sliceOrigin()[2]);
}

inline void mafPipeVisualVTKSliceVolume::setOriginZ(QString stringValue) {
    double *o = m_SlicerPipe->sliceOrigin();
    o[2] = stringValue.toDouble();
    m_SlicerPipe->setSliceOrigin(o);
}

inline void mafPipeVisualVTKSliceVolume::on_originZ_textEdited(QString stringValue) {
    setOriginZ(stringValue);
}

inline QString mafPipeVisualVTKSliceVolume::thickness() {
    return QString::number(m_Thickness);
}

inline void mafPipeVisualVTKSliceVolume::setThickness(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceVolume::on_thickness_textEdited(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceVolume::on_normalX_textEdited(QString stringValue) {
    setNormalX(stringValue);
}

inline void mafPipeVisualVTKSliceVolume::on_normalY_textEdited(QString stringValue) {
    setNormalY(stringValue);
}

inline void mafPipeVisualVTKSliceVolume::on_normalZ_textEdited(QString stringValue) {
    setNormalZ(stringValue);
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

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICEVOLUME_H
