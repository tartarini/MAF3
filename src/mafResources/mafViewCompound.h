/*
 *  mafViewCompound.h
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWCOMPOUND_H
#define MAFVIEWCOMPOUND_H

#include "mafView.h"

namespace mafResources {

/**
 Class name: mafViewCompound
 Base class for compound views.
*/
class MAFRESOURCESSHARED_EXPORT mafViewCompound : public mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewCompound(const QString code_location = "");
    
    /// Create the view
    /*virtual*/ bool initialize();

    /// Show scene node passed as argument.
    /*virtual*/ void showSceneNode(mafSceneNode *node, bool show = true);

    /// clear and delete the SceneGraphs.
    /*virtual*/ void clearScene();

    /// Return the list of sub-views.
    QList<mafView *> *viewList();

protected:
    /// Object destructor.
    /* virtual */ ~mafViewCompound();

private:
    /// Extract all the mafView present in the widget's hierarchy.
    void extractViews(QObject *root);

    QList<mafView *> m_ViewList; ///< List of child views.

public Q_SLOTS:
    /// Update sub-views.
    /*virtual*/ void updateView();
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QList<mafView *> *mafViewCompound::viewList() {
    return &m_ViewList;
}

} // namespace mafResources

#endif // MAFVIEWCOMPOUND_H
