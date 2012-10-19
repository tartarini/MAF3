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

#ifndef MAFTOOLVTKBUTTONSGROUP_H
#define MAFTOOLVTKBUTTONSGROUP_H

// Includes list
#include "mafToolVTKButtonsInterface.h"
#include <QImage>

class msvQVTKButtons;
class vtkQImageToImageSource;
class msvQVTKButtonsGroup;
/**
 Class name: mafToolVTKButtonsGroup
 This is the tool representing a VTK buttons group.
 */
class mafToolVTKButtonsGroup : public mafToolVTKButtonsInterface {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

    /// Allow to execute and update the pipeline when something change.
//     /*virtual*/ void updatePipe(double t = -1);
//     
//     void showTooltip(QString tooltip);
//     
//     void hideTooltip();

public:
     /// Object constructor.
    mafToolVTKButtonsGroup(const QString code_location = "");

    /// add a tool button
    void addButton(mafToolVTKButtonsInterface* button);

    /// remove a tool button
    void removeButton(mafToolVTKButtonsInterface* button);

    /// Allow to take the tool to the initial conditions.
    /*virtual*/ void resetTool();

    /// Update the pipe
    void updatePipe(double t=-1);
  
protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKButtonsGroup();

    msvQVTKButtonsInterface *element();
};

#endif // MAFTOOLVTKBUTTONS_H
