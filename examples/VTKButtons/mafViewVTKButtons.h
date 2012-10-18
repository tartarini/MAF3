/*
 *  mafViewVTKButtons.h
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWVTKBUTTONS_H
#define MAFVIEWVTKBUTTONS_H

// Includes list
#include <mafViewVTK.h>


class mafVTKWidget;
class mafSceneNode;
class mafToolVTKButtonsGroup;


/**
 Class name: mafViewVTKButtons
 This is the VTK MAF3 views.
 */
class mafViewVTKButtons : public mafPluginVTK::mafViewVTK {
    Q_OBJECT

    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafViewVTK);

public:
    /// Object constructor.
    mafViewVTKButtons(const QString code_location = "");

    /// Called to show/hide SceneNode.
    virtual void showSceneNode(mafResources::mafSceneNode *node, bool show = true);

    /// Remove scene node passed as argument.
   /*virtual*/ void removeSceneNode(mafResources::mafSceneNode *node);

   /// Remove vtkButtons.
   virtual void clearScene();

public Q_SLOTS:
    /// Show/hide buttons
    void on_showButtonsCheck_stateChanged(int state);

    /// Show/hide labels
    void on_showLabelsCheck_stateChanged(int state);

    /// Activate "FlyTo" animation
    void on_flyToCheck_stateChanged(int state);

    /// Set position on buttons on corner/center
    void on_positionComboBox_activated(int state);

protected:
    /// Object destructor.
    /* virtual */ ~mafViewVTKButtons();

    /* virtual */ mafResources::mafSceneNode *createSceneNode(mafResources::mafVME *vme);

    mafToolVTKButtonsGroup* m_Group; //singleton

};

#endif // MAFVIEWVTKBUTTONS_H
