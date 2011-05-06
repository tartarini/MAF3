/*
 *  mafInteractionManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERACTIONMANAGER_H
#define MAFINTERACTIONMANAGER_H

#include <mafObjectBase.h>
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafVME;

/**
Class name: mafInteractionManager
Manager class for interaction resource. This class respond to signals coming from the
input device and ask the interactor to perform a pick.
Depending on what has been picked, it checks for a custom interactor style associated
with the picked item and, if any, use it to start, stop and interact with the data item itself.
*/
class MAFRESOURCESSHARED_EXPORT mafInteractionManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafInteractionManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

signals:
    /// left button pressed.
    void leftButtonPressSignal(unsigned long modifier);

    /// left button released.
    void leftButtonReleaseSignal(unsigned long modifier);

    /// middle button pressed.
    void middleButtonPressSignal(unsigned long modifier);

    /// middle button released.
    void middleButtonReleaseSignal(unsigned long modifier);

    /// right button pressed.
    void rightButtonPressSignal(unsigned long modifier);

    /// right button released.
    void rightButtonReleaseSignal(unsigned long modifier);

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

    /// mouse moved.
    void mouseMoveSignal(unsigned long modifier);

    /// mouse wheel moved forward.
    void mouseWheelForwardSignal(unsigned long modifier);

    /// mouse wheel moved backward
    void mouseWheelBackwardSignal(unsigned long modifier);

    /// pick..
    void pickSignal();


private slots:
    /// called when a new VME has been selected.
    void vmeSelected(mafVME *vme);

    /// called when left mouse button is pressed.
    void leftButtonPress(unsigned long modifier);

    /// called when left mouse button is released.
    void leftButtonRelease(unsigned long modifier);

    /// called when middle mouse button is pressed.
    void middleButtonPress(unsigned long modifier);

    /// called when middle mouse button is released.
    void middleButtonRelease(unsigned long modifier);

    /// called when right mouse button is pressed.
    void rightButtonPress(unsigned long modifier);

    /// called when right mouse button is released.
    void rightButtonRelease(unsigned long modifier);

    /// called when left mouse button is pressed.
    void enter();

    /// called when left mouse button is pressed.
    void leave();

    /// called when left mouse button is pressed.
    void keyPress();

    /// called when left mouse button is pressed.
    void keyRelease();

    /// called when left mouse button is pressed.
    void keyChar();

    /// called when left mouse button is pressed.
    void mouseMove(unsigned long modifier);

    /// called when left mouse button is pressed.
    void mouseWheelForward(unsigned long modifier);

    /// called when left mouse button is pressed.
    void mouseWheelBackward(unsigned long modifier);

    /// called when left mouse button is pressed.
    void pick();


private:
    /// Object constructor.
    mafInteractionManager(const QString code_location = "");

    /// Object destructor
    /*virtual*/ ~mafInteractionManager();

    /// Create IDs and connect signals with slots for manage interaction's events.
    void initializeConnections();
};

} // namespace mafResources

#endif // MAFINTERACTIONMANAGER_H
