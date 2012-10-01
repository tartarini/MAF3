/*
 *  mafInteractorVTKPicker.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/07/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACTORVTKPICKER_H
#define MAFINTERACTORVTKPICKER_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafInteractor.h"
#include <mafProxyInterface.h>
#include <vtkCellPicker.h>

namespace mafPluginVTK {

// Class forwarding list

/**
Class name: mafInteractorVTKPicker
This class defines the class for the VTK interactor picker.
@sa mafVME mafInteractor
*/
    class MAFPLUGINVTKSHARED_EXPORT mafInteractorVTKPicker : public mafResources::mafInteractor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInteractor);

public Q_SLOTS:
    /// Called when a VME has been picked.
    /*virtual*/ void mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

public:
    /// Object constructor
    mafInteractorVTKPicker(const QString code_location = "");

    /// Object constructor
    mafInteractorVTKPicker(mafCore::mafProxyInterfacePointer default_is, mafCore::mafProxyInterfacePointer edit_is = NULL, const QString code_location = "");
    
Q_SIGNALS:
        void vmePickedVTKSignal(double *pos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);

protected:
    /// Object destructor.
    /* virtual */ ~mafInteractorVTKPicker();

private:
    vtkCellPicker *m_CellPicker; ///< VTK picker.
};

} // namespace mafPluginVTK

#endif // MAFINTERACTORVTKPICKER_H
