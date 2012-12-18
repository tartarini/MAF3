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

#ifndef MAFIIMPORTEROUTOFCOREVOLUME_H
#define MAFIIMPORTEROUTOFCOREVOLUME_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafImporter.h>

#include "mafVolume.h"
#include <mafProxy.h>

namespace mafPluginOutOfCore {

/**
 Class Name: mafImporterOutOfCoreVolume
 This class allows to import OutOfCore Volume data into a mafVME.
 */
class MAFPLUGINOUTOFCORESHARED_EXPORT mafImporterOutOfCoreVolume : public mafResources::mafImporter {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafImporter);
    
public:
    /// Object constructor.
    mafImporterOutOfCoreVolume(const QString code_location = "");
    
public Q_SLOTS:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporterOutOfCoreVolume();
    
private:
    mafCore::mafProxy<mafVolume> m_ImportedData; ///< Container of the Data Source
};

} // namespace mafPluginVTK


#endif // MAFIIMPORTEROUTOFCOREVOLUME_H
