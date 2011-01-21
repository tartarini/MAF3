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
    mafVTKInteractorPicker(const mafString code_location = "");

    /// Object destructor.
    ~mafVTKInteractorPicker();

    /// Get output of the pipe.
    mafCore::mafContainerInterface *output();

signals:
    /// Signal for VME picked.
    void vmePickSignal(double *picktPos, unsigned long,  mafCore::mafContainerInterface *actor, QEvent *e);

    /// Store last picking point and allow to set a new picking point
    void nextPickSignal();

    /// Remove last picking position stored
    void unDoPickSignal();

public slots:

    /// Set type of parametric surface used as marker.
    void setSurface(mafString parametricSurfaceType);

private slots:
    /// Called when a VME has been picked.
    void vmePick(double *pickPos, unsigned long, mafCore::mafContainerInterface *actor, QEvent * e);

    /// Called when a new pick is required.
    void nextPick();

    /// Called when the operation terminates correctly.
    void OK();


private:
    /// Initialize connection between signals and slots.
    void initializeConnections();

    /// Create choosen surface.
    void internalUpdate();

    /// Initialize and create the pipeline.
    void createPipe();

    /// Set scalar value to input vtkPolyData.
    void setScalarValue(vtkPolyData *data, double scalarValue);

    double *m_Center;

    vtkPolyDataMapper *m_Mapper; ///< Class that maps polygonal data.
    vtkAppendPolyData *m_AppendData; /// Bunch of surfaces.
    mafCore::mafContainer<vtkActor> m_Actor; ///< Output container.
    mafList<double*> m_PointList;
    mafCore::mafContainerInterface *m_Output; ///< Output for visual pipe.
    mafVTKParametricSurface *m_ParametricSurface; ///< Parametric surface used as marker.
    mafString m_ParametricSurfaceType; ///< Type of parametric surface to be used as marker.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafContainerInterface *mafVTKInteractorPicker::output() {
    return m_Output;
}

inline void mafVTKInteractorPicker::setSurface(mafString parametricSurfaceType){
    m_ParametricSurfaceType = parametricSurfaceType;
}


} // namespace mafPluginVTK

#endif // MAFVTKINTERATORPICKER_H
