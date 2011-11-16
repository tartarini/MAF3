/*
 *  mafImporterSTL.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFIMPORTERSTL_H
#define MAFIMPORTERSTL_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafImporter.h>

#include <mafProxy.h>
#include <vtkAlgorithmOutput.h>

class vtkSTLReader;

namespace mafPluginVTK {

/**
 Class Name: mafImporterSTL
 This class allows to import STL data into a mafVME.
 */
class MAFPLUGINVTKSHARED_EXPORT mafImporterSTL : public mafResources::mafImporter {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafImporter);
    
public:
    /// Object constructor.
    mafImporterSTL(const QString code_location = "");
    
public Q_SLOTS:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporterSTL();
    
private:
    mafCore::mafProxy<vtkAlgorithmOutput> m_ImportedData; ///< Container of the Data Source
    vtkSTLReader *m_Reader; ///< Reader of the external data.
};

} // namespace mafPluginVTK


#endif // MAFIMPORTERSTL_H
