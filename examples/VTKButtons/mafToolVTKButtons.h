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

class msvQVTKButtons;
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

    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
    void showTooltip();
    
    void hideTooltip();

public:
    msvQVTKButtons *button();
    
     /// Object constructor.
    mafToolVTKButtons(const QString code_location = "");

    /// Allow to take the tool to the initial conditions.
    /*virtual*/ void resetTool();

    /// Select VME connected to button pressed
    void selectVME();

    
    void setShowButton(bool show);
    bool showButton();    
    void setShowLabel(bool show);
    bool showLabel();
    void setFlyTo(bool active);    
    bool FlyTo();
    void setOnCenter(bool onCenter);
    bool OnCenter();    
  
protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKButtons();

private:
    msvQVTKButtons *m_Button;
};

#endif // MAFTOOLVTKBUTTONS_H
