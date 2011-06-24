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

protected:
    /// Object destructor.
    /* virtual */ ~mafSceneNodeVTK();

private:
};

} //namespace mafPluginVTK

#endif // MAFSCENENODEVTK_H
