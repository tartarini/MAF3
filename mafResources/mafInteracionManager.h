/*
 *  mafInteracionManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACIONMANAGER_H
#define MAFINTERACIONMANAGER_H

#include <mafObjectBase.h>
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafVME;

/**
Class name: mafInteracionManager
This class provides the manager class for MAF3 Interaction.
*/
class MAFRESOURCESSHARED_EXPORT mafInteracionManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafInteracionManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

private slots:
    /// called when selected a vme
    void vmeSelected(mafVME *vme);

private:
    /// Object constructor.
    mafInteracionManager(const mafString code_location = "");

    /// Object destructor
    /*virtual*/ ~mafInteracionManager();

    /// create IDs and connect signals with slots for manage interaction's events.
    void initializeConnections();
};

} // namespace mafResources

#endif // MAFINTERACIONMANAGER_H
