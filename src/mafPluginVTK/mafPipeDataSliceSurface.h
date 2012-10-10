/*
 *  mafPipeDataSliceSurface.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 14/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEDATASLICESURFACE_H
#define MAFPIPEDATASLICESURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafPipeDataSlice.h>
#include <mafProxy.h>
#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>

// Foundation Class forwarding list
class vtkPlane;
class vtkCutter;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafPipeDataSliceSurface
 This class allows you to make a slice on input vtkPolyData.
 */
class MAFPLUGINVTKSHARED_EXPORT mafPipeDataSliceSurface : public mafResources::mafPipeDataSlice {
    Q_OBJECT

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeDataSlice);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

public:
    /// Object constructor.
    mafPipeDataSliceSurface(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeDataSliceSurface();

private:
	vtkSmartPointer<vtkPlane>  m_Plane;  ///< Cutting plane implicit function.
    vtkSmartPointer<vtkCutter> m_Cutter; ///< Cutter fo the input vtkPolyData.
    mafCore::mafProxy<vtkAlgorithmOutput> m_OutputValue; ///< Output value coming from the elaboration data pipe.
};


} // namespace mafPluginVTK

#endif // MAFPIPEDATASLICESURFACE_H
