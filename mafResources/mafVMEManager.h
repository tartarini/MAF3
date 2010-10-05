/*
 *  mafVMEManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVMEMANAGER_H
#define MAFVMEMANAGER_H

#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafVME;

/**
Class name: mafVMEManager
This class provides the manager class for MAF3 VMEs. The manager defines new IDs to add/remove
a VME to/from the hierarchy tree and to select a vme. The IDs are:
- RESOURCES_VME_ADD allows to add a new VME to the hierarchy.
- RESOURCES_VME_REMOVE allows to remove a VME from the hierarchy.
- RESOURCES_VME_SELECT allows to select the active VME on which operate.
- RESOURCES_VME_SELECTED allows to retrieve the current selected VME.
*/
class MAFRESOURCESSHARED_EXPORT mafVMEManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafVMEManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

signals:
    /// Return the current selected vme.
    mafCore::mafObjectBase *selectedVMESignal() const;

    /// Signal emitted when a new VME has to be attached to the hierarchy tree.
    void attachVMEToHierarchy(mafCore::mafObjectBase *vme);

    /// Signal emitted when a VME has to be removed from the hierarchy tree.
    void detachVMEFromHierarchy(mafCore::mafObjectBase *vme);

    /// Signal emitted when a VME has beed selected.
    void selectVME(mafCore::mafObjectBase *vme);

private slots:
    /// Return the current selected vme.
    mafCore::mafObjectBase *selectedVME() const;

    /// Allow to keep track of the selected VME.
    void vmeSelect(mafCore::mafObjectBase *vme);

    /// Add new VME to the system. Each manager than can manage it according to its functionalities.
    void vmeAdd(mafCore::mafObjectBase *vme);

    /// Remove the VME from the system. Each manager then can behave as it can to remove the resource from ite manage.
    void vmeRemove(mafCore::mafObjectBase *vme);

    /// Method called when a VME has been destroyed (by someone else).
    void vmeDestroyed();

private:
    /// Object constructor.
    mafVMEManager(const mafString code_location = "");

    /// create IDs and connect signals with slots for VME add, remove and select.
    void initializeConnections();

    /// Object destructor
    /*virtual*/ ~mafVMEManager();

    /// Remove the VME from the managed VME tree.
    void removeVME(mafVME *vme);

    mafVME *m_SelectedVME; ///< Keep track of the selected VME.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafObjectBase *mafVMEManager::selectedVME() const {
    return (mafCore::mafObjectBase *)m_SelectedVME;
}

} // namespace mafResources

#endif // MAFVMEMANAGER_H
