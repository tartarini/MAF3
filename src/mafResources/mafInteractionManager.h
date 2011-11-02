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
class mafInteractor;

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

Q_SIGNALS:
    /// signals for discover if a vme is picked
    void vmePickSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    
private Q_SLOTS:

    /// called when left mouse button is pressed.
    void mousePress(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);

    /// called when left mouse button is released.
    void mouseRelease(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    
    /// called when mouse is moving.
    void mouseMove(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);

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
    
    /// called when a view is selected, used for initializing the default interactor.
    void setActiveView(mafCore::mafObjectBase *obj);

    /// called when select a vme
    void vmeSelect(mafCore::mafObjectBase *obj);
    
public:
    /// Return an instance of the Manager
    static mafInteractionManager *instance();
    
    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

private:
    /// Object constructor.
    mafInteractionManager(const QString code_location = "");

    /// Object destructor
    /*virtual*/ ~mafInteractionManager();

    /// initialize the manager.
    void initializeConnection();

    mafVME *m_VME; ///< used for retrieve current interactor.
    mafInteractor *m_DefaultInteractor; ///< represents the used interactor by default, without considering vme interaction.
    
};

} // namespace mafResources

#endif // MAFINTERACTIONMANAGER_H
