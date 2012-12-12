/*
 *  mafOperationCTKCommandLineModuleExplorer.h
 *  mafPluginCTK
 *
*  Created by Roberto Mucci and Daniele Giunchi on 16/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafOperationCTKCommandLineModuleExplorer_H
#define mafOperationCTKCommandLineModuleExplorer_H

// Includes list
#include "mafPluginCTKDefinitions.h"
#include <mafImporter.h>
#include <QDialog>


namespace mafPluginCTK {

/**
 Class Name: mafOperationCTKCommandLineModuleExplorer
 This class allows to import Dicom data.
 */
class MAFPLUGINCTKSHARED_EXPORT mafOperationCTKCommandLineModuleExplorer : public mafResources::mafOperation {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);
    
public:
    /// Object constructor.
    mafOperationCTKCommandLineModuleExplorer(const QString code_location = "");

    /// open the dialog for dicom importer
    /*virtual*/ bool initialize();

    static bool acceptObject(mafCore::mafObjectBase *obj);
    
public Q_SLOTS:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafOperationCTKCommandLineModuleExplorer();

    /// Virtual method to implement the cleanup of the operation when it ends.
    /*virtual*/ void terminated();
};

} // namespace mafPluginCTK


#endif // mafOperationCTKCommandLineModuleExplorer_H