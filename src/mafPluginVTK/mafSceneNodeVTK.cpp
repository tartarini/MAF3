/*
 *  mafSceneNodeVTK.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 24/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSceneNodeVTK.h"
#include <mafVME.h>
#include <mafDataSetCollection.h>

#include <vtkAssembly.h>
#include <vtkMatrix4x4.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


mafSceneNodeVTK::mafSceneNodeVTK(const QString code_location) : mafSceneNode(code_location) {
}

mafSceneNodeVTK::mafSceneNodeVTK(mafVME *vme, QObject *graphicObject, const QString visualPipeType, const QString code_location): mafSceneNode(vme, graphicObject, visualPipeType, code_location)  {
    m_Assembly = vtkAssembly::New();
    mafMatrix *matrix = vme->dataSetCollection()->poseMatrix();
    vtkMatrix4x4 *vtkMatrix = vtkMatrix4x4::New();
    vtkMatrix->DeepCopy(matrix->rawData());
    m_Assembly->SetUserMatrix(vtkMatrix);
    vtkMatrix->Delete();
}

mafSceneNodeVTK::~mafSceneNodeVTK() {
    m_Assembly->Delete();
}

void mafSceneNodeVTK::setParentNode(const mafSceneNode *parent) {
    Superclass::setParentNode(parent);
    const mafSceneNodeVTK *node = dynamic_cast<const mafSceneNodeVTK *>(parent);
    if (node) {
        node->nodeAssembly()->AddPart(m_Assembly);
    }
}
