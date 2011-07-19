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

namespace mafResources {

// Class forwarding list
class mafVME;
    
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
    
    /// called when left mouse button is pressed.
    virtual void mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);
    
    /// called when left mouse button is released.
    virtual void mouseRelease(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);
    
    /// called when mouse is moving.
    virtual void mouseMove(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e);
    
    /// mouse enters in the render window.
    virtual void enter(unsigned long modifiers, QEvent *e);
    
    /// mouse exits from the render window.
    virtual void leave(unsigned long modifiers, QEvent *e);
    
    /// press a key char.
    virtual void keyPress(QEvent *e);
    
    /// release a key char.
    virtual void keyRelease(QEvent *e);
    
    /// called when the wheel is moving forward.
    virtual void mouseWheelForward(unsigned long modifiers, QEvent *e);
    
    /// called when the wheel is moving backward.
    virtual void mouseWheelBackward(unsigned long modifiers, QEvent *e);

public:
    /// Object constructor
    mafInteractor(const QString code_location = "");

    /// Object constructor
    //mafInteractor(mafCore::mafProxyInterfacePointer default_is, mafCore::mafProxyInterfacePointer edit_is = NULL, const QString code_location = "");

    /// Allow to assign the default interaction style.
//    void setDefaultInteractionStyle(mafCore::mafProxyInterfacePointer is);

    /// Return the default interaction style.
//    mafCore::mafProxyInterfacePointer defaultInteractionStyle();

    /// Allow to assign the editing interaction style.
//    void setEditingInteractionStyle(mafCore::mafProxyInterfacePointer is);

    /// Return the editing interaction style.
//    mafCore::mafProxyInterfacePointer editingInteractionStyle();
    
    /// set the vme to the interactor
    void setVME(mafVME *vme);
    
    /// set the graphic object to the interactor
    void setGraphicObject(QObject *graphicObject);

protected:
    /// Object destructor.
    /* virtual */ ~mafInteractor();
    
    mafVME *m_VME; ///< vme associated to interactor.
    QObject *m_GraphicObject; ///< represents the graphic object for retrieve useful information.

private:
//    mafCore::mafProxyInterfacePointer m_DefaultInteractorStyle; ///< Default interaction style that will be associated with the mafVME.
//    mafCore::mafProxyInterfacePointer m_EditingInteractorStyle; ///< Interaction style associated to the mafVME and used during the editing operations of the data inside the mafVME itself.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

/*inline mafCore::mafProxyInterfacePointer mafInteractor::defaultInteractionStyle() {
    return m_DefaultInteractorStyle;
}

inline mafCore::mafProxyInterfacePointer mafInteractor::editingInteractionStyle() {
    return m_EditingInteractorStyle;
}*/
    
inline void mafInteractor::setVME(mafVME *vme) {
    m_VME = vme;
}

inline void mafInteractor::setGraphicObject(QObject *graphicObject) {
    m_GraphicObject = graphicObject;
}
    
} // namespace mafResources

#endif // MAFINTERACTOR_H
