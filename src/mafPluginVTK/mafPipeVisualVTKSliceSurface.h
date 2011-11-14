/*
 *  mafPipeVisualVTKSliceSurface.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 09/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEVISUALVTKSLICESURFACE_H
#define MAFPIPEVISUALVTKSLICESURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"
#include "mafPipeDataSliceSurface.h"

#include <vtkSmartPointer.h>

// Foundation Class forwarding list
class vtkPolyDataMapper;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafPipeVisualVTKSliceSurface
 This visual pipe allow to extract a contour from a surface data given a cut origin.
*/
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSliceSurface : public mafPipeVisualVTK {
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
    mafPipeVisualVTKSliceSurface(const QString code_location = "");

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
    /* virtual */ ~mafPipeVisualVTKSliceSurface();

private:
    double m_Thickness; ///< Contour thickness.

    mafPipeDataSliceSurface *m_CutterPipe;    ///< Cutter data pipe.
    vtkSmartPointer<vtkPolyDataMapper> m_Mapper; ///< Data mapper.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QString mafPipeVisualVTKSliceSurface::originX() {
    return QString::number(m_CutterPipe->sliceOrigin()[0]);
}

inline void mafPipeVisualVTKSliceSurface::setOriginX(QString stringValue) {
    double *o = m_CutterPipe->sliceOrigin();
    o[0] = stringValue.toDouble();
    m_CutterPipe->setSliceOrigin(o);
}

inline void mafPipeVisualVTKSliceSurface::on_originX_textEdited(QString stringValue) {
    setOriginX(stringValue);
}

inline QString mafPipeVisualVTKSliceSurface::originY() {
    return QString::number(m_CutterPipe->sliceOrigin()[1]);
}

inline void mafPipeVisualVTKSliceSurface::setOriginY(QString stringValue) {
    double *o = m_CutterPipe->sliceOrigin();
    o[1] = stringValue.toDouble();
    m_CutterPipe->setSliceOrigin(o);
}

inline void mafPipeVisualVTKSliceSurface::on_originY_textEdited(QString stringValue) {
    setOriginY(stringValue);
}

inline QString mafPipeVisualVTKSliceSurface::originZ() {
    return QString::number(m_CutterPipe->sliceOrigin()[2]);
}

inline void mafPipeVisualVTKSliceSurface::setOriginZ(QString stringValue) {
    double *o = m_CutterPipe->sliceOrigin();
    o[2] = stringValue.toDouble();
    m_CutterPipe->setSliceOrigin(o);
}

inline void mafPipeVisualVTKSliceSurface::on_originZ_textEdited(QString stringValue) {
    setOriginZ(stringValue);
}

inline QString mafPipeVisualVTKSliceSurface::thickness() {
    return QString::number(m_Thickness);
}

inline void mafPipeVisualVTKSliceSurface::setThickness(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_thickness_textEdited(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_normalX_textEdited(QString stringValue) {
    setNormalX(stringValue);
}

inline void mafPipeVisualVTKSliceSurface::on_normalY_textEdited(QString stringValue) {
    setNormalY(stringValue);
}

inline void mafPipeVisualVTKSliceSurface::on_normalZ_textEdited(QString stringValue) {
    setNormalZ(stringValue);
}

inline QString mafPipeVisualVTKSliceSurface::normalX() {
    return QString::number(m_CutterPipe->normal()[0]);
}

inline void mafPipeVisualVTKSliceSurface::setNormalX(QString stringValue) {
    double *n = m_CutterPipe->normal();
    n[0] = stringValue.toDouble();
    m_CutterPipe->setNormal(n);
}

inline QString mafPipeVisualVTKSliceSurface::normalY() {
    return QString::number(m_CutterPipe->normal()[1]);
}

inline void mafPipeVisualVTKSliceSurface::setNormalY(QString stringValue) {
    double *n = m_CutterPipe->normal();
    n[1] = stringValue.toDouble();
    m_CutterPipe->setNormal(n);
}

inline QString mafPipeVisualVTKSliceSurface::normalZ() {
    return QString::number(m_CutterPipe->normal()[2]);
}

inline void mafPipeVisualVTKSliceSurface::setNormalZ(QString stringValue) {
    double *n = m_CutterPipe->normal();
    n[2] = stringValue.toDouble();
    m_CutterPipe->setNormal(n);
}

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICESURFACE_H
