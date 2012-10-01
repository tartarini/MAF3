/*
 *  mafInteractorSelection.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 07/10/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACTORSELECTION_H
#define MAFINTERACTORSELECTION_H

// Includes list
#include "mafResourcesDefinitions.h"
#include "mafInteractor.h"
#include <mafProxyInterface.h>

namespace mafResources {

// Class forwarding list

/**
Class name: mafInteractorSelection
This class defines the  class for the interactor selection in MAF3. this is the default interactor for each VME.
@sa mafVME mafInteractor
*/
class MAFRESOURCESSHARED_EXPORT mafInteractorSelection : public mafInteractor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInteractor);

public Q_SLOTS:
    /// Called when a VME has been picked.
    /*virtual*/ void mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

public:
    /// Object constructor
    mafInteractorSelection(const QString code_location = "");

    /// Object constructor
//    mafInteractorSelection(mafCore::mafProxyInterfacePointer default_is, mafCore::mafProxyInterfacePointer edit_is = NULL, const QString code_location = "");
    
protected:
    /// Object destructor.
    /* virtual */ ~mafInteractorSelection();
};

} // namespace mafResources

#endif // MAFINTERACTORSELECTION_H
