/*
 *  mafVMEManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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
This class provides the management interface for MAF3 VMEs. The manager defines topics to add/remove
a VME to/from the hierarchy tree and to select a vme. The topics are:
- maf.local.resources.vme.add which allows to add a new VME to the hierarchy.
- maf.local.resources.vme.remove which allows to remove a VME from the hierarchy.
- maf.local.resources.vme.reparent which allows to reparent a VME to another.
- maf.local.resources.vme.select which allows to select the active VME on which operate.
- maf.local.resources.vme.selected which allows to retrieve the current selected VME.
- maf.local.resources.hierarchy.request which allows to request the hierarchy of the vmes
- maf.local.resources.hierarchy.new which allows to create a new hierarchy deleting the old one.
- maf.local.resources.hierarchy.root retrieve root vme

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

    /// Return hierarchy indexing the mafVME
    mafCore::mafHierarchy *hierarchy() const;

Q_SIGNALS:
    /// Return the current selected vme.
    mafCore::mafObjectBase *selectedVMESignal() const;

    /// Signal emitted when a new VME has to be attached to the hierarchy tree.
    void attachVMEToHierarchy(mafCore::mafObjectBase *vme);

    /// Signal emitted when a VME has to be removed from the hierarchy tree.
    void detachVMEFromHierarchy(mafCore::mafObjectBase *vme);

    /// Signal emitted when a VME has to be removed from the hierarchy tree.
    void reparentVMESignal(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent);

    /// Signal emitted when a VME has been selected.
    void selectVME(mafCore::mafObjectBase *vme);

    /// Request hierarchy of VMEs.
    mafCore::mafHierarchyPointer requestVMEHierarchySignal();

    /// Return a new hierarchy deleting the old one.
    mafCore::mafHierarchyPointer newVMEHierarchySignal();

    /// Signal for returning the root
    mafCore::mafObject *rootSignal() const;
    
    /// Signal for calculating absolute pose matrix of a VME
    mafResources::mafMatrixPointer absolutePoseMatrixSignal(mafCore::mafObjectBase *vme);

private Q_SLOTS:
    /// Return the current selected vme.
    mafCore::mafObjectBase *selectedVME() const;

    /// Allow to keep track of the selected VME.
    void vmeSelect(mafCore::mafObjectBase *vme);

    /// Add new VME to the system. Each manager than can manage it according to its functionalities.
    void vmeAdd(mafCore::mafObjectBase *vme);

    /// Remove the VME from the system. Each manager then can behave as it can to remove the resource from ite manage.
    void vmeRemove(mafCore::mafObjectBase *vme);

    /// Reparent a VME to another.
    void vmeReparent(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent);

    /// Method called when a VME has been destroyed (by someone else).
    void vmeDestroyed();

    /// Create a hierarchy of VMEs
    mafCore::mafHierarchyPointer requestVMEHierarchy();

    /// Return a new hierarchy deleting the old one.
    mafCore::mafHierarchyPointer newVMEHierarchy();
    
    /// Return a current root
    mafCore::mafObject *root() const;
    
    /// Return absolute pose matrix of a VME.
    mafResources::mafMatrixPointer absolutePoseMatrix(mafCore::mafObjectBase *vme);

protected:
    /// Object destructor
    /*virtual*/ ~mafVMEManager();

private:
    /// Object constructor.
    mafVMEManager(const QString code_location = "");

    /// create IDs and connect signals with slots for VME add, remove and select.
    void initializeConnections();

    /// Remove the VME from the managed VME tree.
//    void removeVME(mafVME *vme);

    mafCore::mafObject *m_SelectedVME; ///< Keep track of the selected VME.
    mafCore::mafObject *m_Root; ///< Root of the tree.
    mafCore::mafHierarchy *m_VMEHierarchy; ///< Hierarchy indicizing the mafVME.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafObjectBase *mafVMEManager::selectedVME() const {
    return (mafCore::mafObjectBase *)m_SelectedVME;
}

inline mafCore::mafHierarchy *mafVMEManager::hierarchy() const {
    return m_VMEHierarchy;
}
    
inline mafCore::mafObject *mafVMEManager::root() const {
    return m_Root;
}

} // namespace mafResources

#endif // MAFVMEMANAGER_H
