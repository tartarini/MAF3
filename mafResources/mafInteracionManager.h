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
    void leftButtonPressSignal();

    /// left button released.
    void leftButtonReleaseSignal();

    /// middle button pressed.
    void middleButtonPressSignal();

    /// middle button released.
    void middleButtonReleaseSignal();

    /// right button pressed.
    void rightButtonPressSignal();

    /// right button released.
    void rightButtonReleaseSignal();

    /// enter..
    void enterSignal();

    /// leave..
    void leaveSignal();

    /// key pressed
    void keyPressSignal();

    /// key released
    void keyReleaseSignal();

    /// char..
    void charSignal();

    /// timer..
    void timerSignal();

    /// mouse moved.
    void mouseMoveSignal();

    /// mouse wheel mouved forward.
    void mouseWheelForwardSignal();

    /// mouse wheel mouved backward
    void mouseWheelBackwardSignal();

    /// pick..
    void pickSignal();


private slots:
    /// called when a new VME has been selected.
    void vmeSelected(mafVME *vme);

    /// called when left mouse button is pressed.
    void leftButtonPress();

    /// called when left mouse button is released.
    void leftButtonRelease();

    /// called when middle mouse button is pressed.
    void middleButtonPress();

    /// called when middle mouse button is relesed.
    void middleButtonRelease();

    /// called when right mouse button is pressed.
    void rightButtonPress();

    /// called when right mouse button is released.
    void rightButtonRelease();

    /// called when left mouse button is pressed.
    void enter();

    /// called when left mouse button is pressed.
    void leave();

    /// called when left mouse button is pressed.
    void keyPress();

    /// called when left mouse button is pressed.
    void keyRelease();

    /// called when left mouse button is pressed.
    void char();

    /// called when left mouse button is pressed.
    void timer();

    /// called when left mouse button is pressed.
    void mouseMove();

    /// called when left mouse button is pressed.
    void mouseWheelForward();

    /// called when left mouse button is pressed.
    void mouseWheelBackward();

    /// called when left mouse button is pressed.
    void pick();


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
