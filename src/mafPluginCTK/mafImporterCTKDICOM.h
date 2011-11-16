/*
 *  mafImporterCTKDICOM.h
 *  mafPluginCTK
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFIMPORTERCTKDICOM_H
#define MAFIMPORTERCTKDICOM_H

// Includes list
#include "mafPluginCTKDefinitions.h"
#include <mafImporter.h>

namespace mafPluginCTK {

/**
 Class Name: mafImporterCTKDICOM
 This class allows to import VTK data into a mafVME.
 */
class MAFPLUGINCTKSHARED_EXPORT mafImporterCTKDICOM : public mafResources::mafImporter {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafImporter);
    
public:
    /// Object constructor.
    mafImporterCTKDICOM(const QString code_location = "");

    /// open the dialog for dicom importer
    /*virtual*/ bool initialize();
    
public Q_SLOTS:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporterCTKDICOM();
    
private:
    //mafCore::mafProxy<vtkAlgorithmOutput> m_ImportedData; ///< Container of the Data Source
    //vtkDataSetReader *m_Reader; ///< Reader of the external data.
};

} // namespace mafPluginCTK


#endif // MAFIMPORTERCTKDICOM_H