/*
 *  mafSceneNodeVTK.h
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 24/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCENENODEVTK_H
#define MAFSCENENODEVTK_H

#include "mafPluginVTKDefinitions.h"
#include <mafSceneNode.h>

class vtkAssembly;

namespace mafPluginVTK {

/**
Class name: mafSceneNodeVTK
This class represents a scene node for third party library VTK.
*/
class MAFPLUGINVTKSHARED_EXPORT mafSceneNodeVTK : public mafResources::mafSceneNode {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafSceneNode);
public:
    /// Object constructor. USE THE CONSTRUCTOR WITH VME AS PARAMETER.
    mafSceneNodeVTK(const QString code_location = "");

    /// Object constructor.
    mafSceneNodeVTK(mafResources::mafVME *vme, QObject *graphicObject, const QString visualPipeType, const QString code_location = "");

    /// Set the parent of the current node.
    /*virtual*/ void setParentNode(const mafResources::mafSceneNode *parent);

    /// Return the node assembly.
    vtkAssembly *nodeAssembly() const;

protected:
    /// Object destructor.
    /* virtual */ ~mafSceneNodeVTK();

private:
    vtkAssembly *m_Assembly; ///< Assembly containing the prop 3D to visualize.

public Q_SLOTS:
    /// update the position of the vme.
    void update();
    
    /// Set the visibility of its rendering scene.
    /*virtual*/ void setVisibility(bool visible);

};

inline vtkAssembly *mafSceneNodeVTK::nodeAssembly() const {
    return m_Assembly;
}

} //namespace mafPluginVTK

#endif // MAFSCENENODEVTK_H
