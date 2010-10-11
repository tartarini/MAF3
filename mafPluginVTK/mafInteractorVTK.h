/*
 *  mafInteractorVTK.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 07/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACTORVTK_H
#define MAFINTERACTORVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"

// Class forwarding list
class vtkInteractorStyle;


namespace mafPluginVTK {

class MAFPLUGINVTKSHARED_EXPORT mafInteractorVTK : public mafResources::mafInteractor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInteractor);

public:
    mafInteractorVTK(const mafString code_location = "");

private:
    mafCore::mafContainer<vtkInteractorStyle> *m_InteractorStyle;  ///< VTK Default interaction style.


};

} //namespace mafPluginVTK

#endif // MAFINTERACTORVTK_H
