/*
 *  mafOperationVTKAddLandmark.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/07/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONVTKADDLANDAMARK_H
#define MAFOPERATIONVTKADDLANDAMARK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafOperation.h"
#include "mafVTKParametricSurfaceSphere.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;
class vtkAppendPolyData;
class vtkRenderer;

namespace mafPluginVTK {

/**
Class name: mafOperationVTKAddLandmark
This class represent an operation which add landamrk on a surface.
*/

class MAFPLUGINVTKSHARED_EXPORT mafOperationVTKAddLandmark : public mafResources::mafOperation {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    mafOperationVTKAddLandmark(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Initialize the operation. Put here the initialization of operation's parameters.
    /*virtual*/ bool initialize();

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
    /// Execute the operation.
    /*virtual*/ void execute();

    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();

    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();

    /// Called when a VME has been picked.
    /*virtual*/ void vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

    /// Set operation parameters.
    /*virtual*/ void setParameters(QVariantList parameters);

protected:
    /// Terminate the operation's execution.
    /*virtual*/ void terminated();

    /// Object destructor.
    /* virtual */~mafOperationVTKAddLandmark();

private slots:
    /// Called when a new pick is required.
    void nextPick();

private:
    /// Initialize connection between signals and slots.
    void initializeConnections();

    /// Create chosen surface.
    void internalUpdate();

    /// Set scalar value to input vtkPolyData.
    void setScalarValue(vtkPolyData *data, double scalarValue);

    double *m_Center; ///< point in which the pick happens.
    vtkAppendPolyData *m_AppendData; /// Bunch of surfaces.
    QList<double*> m_PointList;
    QList<mafResources::mafVME*> m_VMEList;
    mafVTKParametricSurfaceSphere *m_ParametricSphere; ///< Parametric surface used as marker.
    mafResources::mafVME *m_VME; ///< Output VME;
    mafResources::mafDataSet *m_DataSet; ///< Dataset of output VME;
    mafCore::mafProxy<vtkAlgorithmOutput> m_LandmarkContainer; ///< output of the operation.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

} // namespace mafPluginVTK

#endif // MAFOPERATIONVTKADDLANDAMARK_H
