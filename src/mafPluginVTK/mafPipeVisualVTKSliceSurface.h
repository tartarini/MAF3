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
#include "mafPipeVisualVTKSlice.h"
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
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSliceSurface : public mafPipeVisualVTKSlice {
    Q_OBJECT
    Q_PROPERTY(QString sliceAxes READ sliceAxes WRITE setSliceAxes)
    Q_PROPERTY(QString thickness READ thickness WRITE setThickness)

    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTKSlice);

public:
    /// Object constructor;
    mafPipeVisualVTKSliceSurface(const QString code_location = "");

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

    /// Set the slice axes normal to the slicing plane.
    QString sliceAxes() const;

    /// Assign the slice axes normal to the slicing plane.
    void setSliceAxes(QString axes);

    /// Set normalX value from text box.
    void on_normalX_textEdited(QString stringValue);

    /// Set normalY value from text box.
    void on_normalY_textEdited(QString stringValue);

    /// Set normalZ value from text box.
    void on_normalZ_textEdited(QString stringValue);

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

    mafPipeDataSliceSurface *m_SlicerPipe;    ///< Cutter data pipe.
    vtkSmartPointer<vtkPolyDataMapper> m_Mapper; ///< Data mapper.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QString mafPipeVisualVTKSliceSurface::thickness() {
    return QString::number(m_Thickness);
}

inline void mafPipeVisualVTKSliceSurface::setThickness(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_thickness_textEdited(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_originX_textEdited(QString stringValue) {
    m_Origin->setX(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceSurface::on_originY_textEdited(QString stringValue) {
    m_Origin->setY(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceSurface::on_originZ_textEdited(QString stringValue) {
    m_Origin->setZ(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceSurface::on_normalX_textEdited(QString stringValue) {
    m_Normal->setX(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceSurface::on_normalY_textEdited(QString stringValue) {
    m_Normal->setY(stringValue.toDouble());
    setModified();
}

inline void mafPipeVisualVTKSliceSurface::on_normalZ_textEdited(QString stringValue) {
    m_Normal->setZ(stringValue.toDouble());
    setModified();
}

inline QString mafPipeVisualVTKSliceSurface::sliceAxes() const {
    return QString::number(m_SlicerPipe->planeNormalAxes());
}

inline void mafPipeVisualVTKSliceSurface::setSliceAxes(QString axes) {
    mafResources::mafPlaneNormal n = (mafResources::mafPlaneNormal)axes.toInt();
    m_SlicerPipe->setPlaneNormalAxes(n);
}

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICESURFACE_H
