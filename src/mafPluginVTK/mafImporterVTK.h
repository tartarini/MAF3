/*
 *  mafImporterVTK.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFIMPORTERVTK_H
#define MAFIMPORTERVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafImporter.h>

#include <mafProxy.h>
#include <vtkAlgorithmOutput.h>

class vtkDataSetReader;

namespace mafPluginVTK {

/**
 Class Name: mafImporterVTK
 This class allows to import VTK data into a mafVME.
 */
class MAFPLUGINVTKSHARED_EXPORT mafImporterVTK : public mafResources::mafImporter {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafImporter);
    
public:
    /// Object constructor.
    mafImporterVTK(const QString code_location = "");
    
public Q_SLOTS:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporterVTK();
    
private:
    mafCore::mafProxy<vtkAlgorithmOutput> m_ImportedData; ///< Container of the Data Source
    vtkDataSetReader *m_Reader; ///< Reader of the external data.
};

} // namespace mafPluginVTK


#endif // MAFIMPORTERVTK_H