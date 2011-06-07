/*
 *  mafVTKInteractorPicker.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/12/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKINTERACTORPICKER_H
#define MAFVTKINTERACTORPICKER_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafVTKParametricSurface.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;
class vtkAppendPolyData;

class vtkRenderer;

namespace mafPluginVTK {

/**
Class name: mafVTKInteractorPicker
This class represent an interactor implementing a picking operation.
*/

class MAFPLUGINVTKSHARED_EXPORT mafVTKInteractorPicker : public mafResources::mafInteractor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInteractor);

public:
    /// Object constructor.
    mafVTKInteractorPicker(const QString code_location = "");

    /// Get output of the pipe.
    vtkActor *output();

signals:
    /// Signal for VME picked.
    void vmePickedSignal(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj);

    /// Store last picking point and allow to set a new picking point
    void nextPickSignal();

    /// Remove last picking position stored
    void unDoPickSignal();
    
    /// Remove last picking position stored
    void OKSignal();

public slots:
    /// Called when a VME has been picked.
    /*virtual*/ void vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj);

    /// Set type of parametric surface used as marker.
    void setSurface(QString parametricSurfaceType);

protected:
    /// Object destructor.
    /* virtual */~mafVTKInteractorPicker();

private slots:
    /// Called when a new pick is required.
    void nextPick();

    /// Called when the operation terminates correctly.
    void OK();

private:
    /// Initialize connection between signals and slots.
    void initializeConnections();

    /// Create chosen surface.
    void internalUpdate();

    /// Set scalar value to input vtkPolyData.
    void setScalarValue(vtkPolyData *data, double scalarValue);

    double *m_Center; ///< point in which the pick happens.
    vtkPolyDataMapper *m_Mapper; ///< Class that maps polygonal data.
    vtkAppendPolyData *m_AppendData; /// Bunch of surfaces.
    QList<double*> m_PointList;
    vtkActor *m_Output; ///< Output for visual pipe.
    mafVTKParametricSurface *m_ParametricSurface; ///< Parametric surface used as marker.
    QString m_ParametricSurfaceType; ///< Type of parametric surface to be used as marker.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline vtkActor *mafVTKInteractorPicker::output() {
    return m_Output;
}

inline void mafVTKInteractorPicker::setSurface(QString parametricSurfaceType){
    m_ParametricSurfaceType = parametricSurfaceType;
}


} // namespace mafPluginVTK

#endif // MAFVTKINTERATORPICKER_H
