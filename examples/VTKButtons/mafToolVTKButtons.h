/*
 *  mafToolVTKButtons.h
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 13/01/12.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTKBUTTONS_H
#define MAFTOOLVTKBUTTONS_H

// Includes list
#include <mafToolVTK.h>

class vtkButtonWidget;

/**
 Class name: mafToolVTKButtons
 This is the tool representing a VTK buttons.
 */
class mafToolVTKButtons : public mafPluginVTK::mafToolVTK {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

public:
     /// Object constructor.
    mafToolVTKButtons(const QString code_location = "");

    /// Allow to take the tool to the initial conditions.
    /*virtual*/ void resetTool();

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKButtons();

private:
    vtkButtonWidget *m_ButtonWidget; ///< VTK button widget
};


#endif // MAFTOOLVTKBUTTONS_H
