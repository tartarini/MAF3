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
Manager class for interaction resource. This class respond to signals coming from the
input device and ask the interactor to perform a pick.
Depending on what has been picked, it checks for a custom interactor style associated
with the picked item and, if any, use it to start, stop and interact with the data item itself.
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

signals:
    /// left button pressed.
    void leftButtonPressVTKSignal();

    /// left button released.
    void leftButtonReleaseVTKSignal();

    /// middle button pressed.
    void middleButtonPressVTKSignal();

    /// middle button released.
    void middleButtonReleaseVTKSignal();

    /// right button pressed.
    void rightButtonPressVTKSignal();

    /// right button released.
    void rightButtonReleaseVTKSignal();

    /// enter..
    void enterVTKSignal();

    /// leave..
    void leaveVTKSignal();

    /// key pressed
    void keyPressVTKSignal();

    /// key released
    void keyReleaseVTKSignal();

    /// char..
    void charVTKSignal();

    /// timer..
    void timerVTKSignal();

    /// mouse moved.
    void mouseMoveVTKSignal();

    /// mouse wheel mouved forward.
    void mouseWheelForwardVTKSignal();

    /// mouse wheel mouved backward
    void mouseWheelBackwardVTKSignal();

    /// pisck..
    void pickVTKSignal();


private slots:
    /// called when a new VME has been selected.
    void vmeSelected(mafVME *vme);

    /// called when left mouse button is pressed.
    void leftButtonPress();

private:
    /// Object constructor.
    mafInteracionManager(const mafString code_location = "");

    /// Object destructor
    /*virtual*/ ~mafInteracionManager();

    /// Create IDs and connect signals with slots for manage interaction's events.
    void initializeConnections();
};

} // namespace mafResources

#endif // MAFINTERACIONMANAGER_H
