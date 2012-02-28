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

    /// Set the slice axes normal to the slicing plane.
//     QString sliceAxes() const;

    /// Assign the slice axes normal to the slicing plane.
//     void setSliceAxes(QString axes);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSlice();

    mafResources::mafPoint *m_Normal; ///< Normal of the slicing plane.
    mafResources::mafPoint *m_Origin; ///< Origin of the slicing plane.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

// inline QString mafPipeVisualVTKSlice::sliceAxes() const {
//     return QString::number(m_SlicerPipe->planeNormalAxes());
// }

// inline void mafPipeVisualVTKSlice::setSliceAxes(QString axes) {
//     mafResources::mafPlaneNormal n = (mafResources::mafPlaneNormal)axes.toInt();
//     m_SlicerPipe->setPlaneNormalAxes(n);
// }

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICEVOLUME_H
