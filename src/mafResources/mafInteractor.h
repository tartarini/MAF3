/*
 *  mafInteractor.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 07/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACTOR_H
#define MAFINTERACTOR_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafProxyInterface.h>
#include <mafProxy.h>
#include <mafProxyInterface.h>

namespace mafResources {

// Class forwarding list

/**
Class name: mafInteractor
This class defines the base class for the interactor in MAF3.
The interactor can have optionally a default style that will be considered during the VME selection
from the input device and optionally an editing style which can be used during the editing operation of the data associated with the VME.
@sa mafVME
*/
class MAFRESOURCESSHARED_EXPORT mafInteractor : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public slots:
    /// Called when a VME has been picked.
    virtual void vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

public:
    /// Object constructor
    mafInteractor(const QString code_location = "");

    /// Object constructor
    mafInteractor(mafCore::mafProxyInterfacePointer default_is, mafCore::mafProxyInterfacePointer edit_is = NULL, const QString code_location = "");

    /// Allow to assign the default interaction style.
    void setDefaultInteractionStyle(mafCore::mafProxyInterfacePointer is);

    /// Return the default interaction style.
    mafCore::mafProxyInterfacePointer defaultInteractionStyle();

    /// Allow to assign the editing interaction style.
    void setEditingInteractionStyle(mafCore::mafProxyInterfacePointer is);

    /// Return the editing interaction style.
    mafCore::mafProxyInterfacePointer editingInteractionStyle();

protected:
    /// Object destructor.
    /* virtual */ ~mafInteractor();

private:
    mafCore::mafProxyInterfacePointer m_DefaultInteractorStyle; ///< Default interaction style that will be associated with the mafVME.
    mafCore::mafProxyInterfacePointer m_EditingInteractorStyle; ///< Interaction style associated to the mafVME and used during the editing operations of the data inside the mafVME itself.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafProxyInterfacePointer mafInteractor::defaultInteractionStyle() {
    return m_DefaultInteractorStyle;
}

inline mafCore::mafProxyInterfacePointer mafInteractor::editingInteractionStyle() {
    return m_EditingInteractorStyle;
}

} // namespace mafResources

#endif // MAFINTERACTOR_H
