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

private slots:

    /// called when left mouse button is pressed.
    void buttonPress(double *pos, unsigned long modifiers, QEvent *e);

    /// called when left mouse button is released.
    void buttonRelease(double *pos, unsigned long modifiers, QEvent *e);
    
    /// called when mouse is moving.
    void mouseMove(double *pos, unsigned long modifiers, QEvent *e);

    /// mouse enters in the render window.
    void enter(unsigned long modifiers, QEvent *e);

    /// mouse exits from the render window.
    void leave(unsigned long modifiers, QEvent *e);

    /// press a key char.
    void keyPress(QEvent *e);

    /// release a key char.
    void keyRelease(QEvent *e);
    
    /// called when the wheel is moving forward.
    void mouseWheelForward(unsigned long modifiers, QEvent *e);

    /// called when the wheel is moving backward.
    void mouseWheelBackward(unsigned long modifiers, QEvent *e);
    
    /// called when a vme has been picked
    void vmePicked(double *pos, unsigned long modifiers, mafVME *vme, QEvent *e);


private:
    /// Object constructor.
    mafInteractionManager(const QString code_location = "");

    /// Object destructor
    /*virtual*/ ~mafInteractionManager();

    /// initialize the manager.
    void initialize();
};

} // namespace mafResources

#endif // MAFINTERACTIONMANAGER_H
