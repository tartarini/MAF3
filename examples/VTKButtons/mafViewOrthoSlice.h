/*
 *  mafViewOrthoSlice.h
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafView.h>
 
/**
 Class name: mafViewOrthoSlice
 mafView compound with 4 panels containing 4 rendering windows.
*/
class mafViewOrthoSlice : public mafResources::mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewOrthoSlice(const QString code_location = "");
    
    /// Create the view
    /*virtual*/ bool initialize();

    /// Remove scene node passed as argument.
    /*virtual*/ void removeSceneNode(mafResources::mafSceneNode *node);

    /// Show scene node passed as argument.
    /*virtual*/ void showSceneNode(mafResources::mafSceneNode *node, bool show = true);

    /// Select a scene node.
    /*virtual*/ void selectSceneNode(mafResources::mafSceneNode *node, bool select);

    /// factory method for creating SceneNode VTK.
    /*virtual*/ mafResources::mafSceneNode *createSceneNode(mafResources::mafVME *vme);

protected:
    /// Object destructor.
    /* virtual */ ~mafViewOrthoSlice();

private:
    /// Extract all the mafView present in the widget's hierarchy.
    void extractViews(QObject *root);

    QList<mafResources::mafView *> m_ViewList; ///< List of child views.

Q_SIGNALS:
    /// Signal used to propagate the mafSceneNode visibility.
    void propagateSceneNodeShowSignale();

public Q_SLOTS:
    /// Update sub-views.
    /*virtual*/ void updateView();
};
