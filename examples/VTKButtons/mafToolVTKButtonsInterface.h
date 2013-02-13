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

#ifndef MAFTOOLVTKBUTTONSINTERFACE_H
#define MAFTOOLVTKBUTTONSINTERFACE_H

// Includes list
#include <mafToolVTK.h>
#include <QImage>

class msvQVTKButtonsInterface;
class vtkQImageToImageSource;
class msvQVTKButtonsGroup;
/**
 Class name: mafToolVTKButtonsInterface
 This is the tool representing a VTK buttons group.
 */
class mafToolVTKButtonsInterface : public mafPluginVTK::mafToolVTK {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    virtual void graphicObjectInitialized()=0;

    /// Allow to execute and update the pipeline when something change.
    virtual void updatePipe(double t = -1)=0;
    
    void showTooltip(QString tooltip);
    
    void hideTooltip();

public:
     /// Object constructor.
    mafToolVTKButtonsInterface(const QString code_location = "");

    /// Allow to take the tool to the initial conditions.
    virtual void resetTool()=0;

    /// Select VME connected to button pressed
    void selectVME();

    /// Set the visibility of its rendering scene.
    /*virtual*/ void setVisibility(bool visible);

    /// show label for the Buttons, use update to renderize.
    void setShowLabel(bool show);
    /// retrieve show status of the label.
    bool showLabel();

    virtual msvQVTKButtonsInterface *element()=0;
  
protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKButtonsInterface();

    msvQVTKButtonsInterface* m_Element;

private:    
    bool isLoaded;
};

#endif // MAFTOOLVTKBUTTONSINTERFACE_H
