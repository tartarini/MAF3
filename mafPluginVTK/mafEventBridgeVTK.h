/*
 *  mafEventBridgeVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFEVENTBRIDGEVTK_H
#define MAFEVENTBRIDGEVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <vtkEventQtSlotConnect.h>

// Class forwarding list
class vtkRenderWindowInteractor;

namespace mafPluginVTK {

class MAFPLUGINVTKSHARED_EXPORT mafEventBridgeVTK : public mafCore::mafObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);
public:
    /// Object constructor.
    mafEventBridgeVTK(const mafString code_location = "");

    /// Object constructor.
    mafEventBridgeVTK(vtkRenderWindowInteractor *inter, const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafEventBridgeVTK();

    /// Set vtkInteractor
    void setInteractor(vtkRenderWindowInteractor *inter);

    /// Check if VME has been picked
    bool vmePickCheck();



public slots:

    /// Emit signal on VTKCommand event.
    void leftButtonPressEvent();

    /// Emit signal on VTKCommand event
    void leftButtonReleaseEvent();

    /// Emit signal on VTKCommand event
    void middleButtonPressEvent();

    /// Emit signal on VTKCommand event
    void middleButtonReleaseEvent();

    /// Emit signal on VTKCommand event
    void rightButtonPressEvent();

    /// Emit signal on VTKCommand event
    void rightButtonReleaseEvent();

    /// Emit signal on VTKCommand event
    void enterEvent();

    /// Emit signal on VTKCommand event
    void leaveEvent();

    /// Emit signal on VTKCommand event
    void keyPressEvent();

    /// Emit signal on VTKCommand event
    void keyReleaseEvent();

    /// Emit signal on VTKCommand event
    void charEvent();

    /// Emit signal on VTKCommand event
    void mouseMoveEvent();

    /// Emit signal on VTKCommand event
    void mouseWheelForwardEvent();

    /// Emit signal on VTKCommand event
    void mouseWheelBackwardEvent();

    /// Emit signal on VTKCommand event
    void pickEvent();


private:
    /// Create connection between VTK and QT.
    void initializeEventBridge();

    /// Get key modifiers.
    void getModifiers();

    vtkEventQtSlotConnect *m_Connections; ///< Creates a connection between VTK and Qt.
    vtkRenderWindowInteractor *m_Interactor; ///< Represent the vtkRenderWindowInteractor.
    unsigned long m_Modifiers;  ///< Optional modifiers for the button.
};

} // namespace mafPluginVTK

#endif // MAFEVENTBRIDGEVTK_H
