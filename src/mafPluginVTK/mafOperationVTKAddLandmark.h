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

    Q_PROPERTY(double radius READ radius WRITE setRadius)

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
    
    /// Button to insert a new landmark.
    void on_nextButton_released();
    
    /// Button to remove last inserted new landmark.
    void on_removeButton_released();
    
    /// Set landmark radius.
    void on_ALRadius_valueChanged(double radius);
    
private slots:
    /// Called when a new insert landmark is required.
    void fixLandmark();

    
signals:
    /// Signal for VME picked.
    void vmePickedSignal(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj);

public:
    /// Object constructor.
    mafOperationVTKAddLandmark(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Initialize the operation. Put here the initialization of operation's parameters.
    /*virtual*/ bool initialize();
    
    /// Return the landmark radius.
    double radius() const;
    
    /// Assign the radius to the landmark.
    void setRadius(double r);

protected:
    /// Terminate the operation's execution.
    /*virtual*/ void terminated();

    /// Object destructor.
    /* virtual */~mafOperationVTKAddLandmark();

private:
    /// Initialize connection between signals and slots.
    void initializeConnections();

    /// Create chosen surface.
    void internalUpdate();

    /// Remove landmark.
    void removeLandmark(int index);

    /// Set scalar value to input vtkPolyData.
    void setScalarValue(vtkPolyData *data, double scalarValue);

    double *m_Center; ///< point in which the pick happens.
    double m_LandmarkRadius; ///< Landamrk radius.
    QList<mafResources::mafVME*> m_VMEList;
    mafVTKParametricSurfaceSphere *m_ParametricSphere; ///< Parametric surface used as marker.
    QList<mafCore::mafProxy<vtkAlgorithmOutput> > m_LandmarkContainerList; ///< List of container of landmark data.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

} // namespace mafPluginVTK

#endif // MAFOPERATIONVTKADDLANDAMARK_H
