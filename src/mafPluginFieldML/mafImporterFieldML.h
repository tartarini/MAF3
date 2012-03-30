/*
 *  mafImporterFieldML.h
 *  mafPluginFieldML
 *
 *  Created by Paolo Quadrani on 16/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFIMPORTERFIELDML_H
#define MAFIMPORTERFIELDML_H

// Includes list
#include "mafPluginFieldMLDefinitions.h"
#include <mafImporter.h>

#include <mafProxy.h>

namespace mafPluginFieldML {

/**
 Class Name: mafImporterFieldML
 This class allows to import STL data into a mafVME.
 */
class MAFPLUGINFIELDMLSHARED_EXPORT mafImporterFieldML : public mafResources::mafImporter {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafImporter);
    
public:
    /// Object constructor.
    mafImporterFieldML(const QString code_location = "");
    
public Q_SLOTS:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporterFieldML();
    
private:
    //mafCore::mafProxy<vtkAlgorithmOutput> m_ImportedData; ///< Container of the Data Source
    //vtkSTLReader *m_Reader; ///< Reader of the external data.
};

} // namespace mafPluginVTK


#endif // MAFIMPORTERFIELDML_H
