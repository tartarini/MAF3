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
#include "mafVTKWidget.h"
#include <mafPipeVisual.h>

#include <mafVME.h>
#include <mafDataSetCollection.h>

#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkProp3D.h>
#include <vtkProp3DCollection.h>
#include <vtkAssembly.h>
#include <vtkMatrix4x4.h>

#include <QDebug>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


mafSceneNodeVTK::mafSceneNodeVTK(const QString code_location) : mafSceneNode(code_location) {
}

mafSceneNodeVTK::mafSceneNodeVTK(mafVME *vme, QObject *graphicObject, const QString visualPipeType, const QString code_location): mafSceneNode(vme, graphicObject, visualPipeType, code_location)  {
    m_Assembly = vtkAssembly::New();
    m_Assembly->SetPickable(1);
    update();
    
    connect(vme->dataSetCollection(), SIGNAL(modifiedObject()), this, SLOT(update()), Qt::DirectConnection);
}

mafSceneNodeVTK::~mafSceneNodeVTK() {
    m_Assembly->Delete();
}

void mafSceneNodeVTK::setParentNode(const mafSceneNode *parent) {
    //remove assembly from old parent
    const mafSceneNodeVTK *parentNodeVTK;
    if(parentNode()) {
        parentNodeVTK = dynamic_cast<const mafSceneNodeVTK *>(parentNode());
        parentNodeVTK->nodeAssembly()->RemovePart(m_Assembly);
    }
    
    Superclass::setParentNode(parent);
    parentNodeVTK = dynamic_cast<const mafSceneNodeVTK *>(parent);
    if (parentNodeVTK) {
        update();
        parentNodeVTK->nodeAssembly()->AddPart(m_Assembly);
    }
    
    if(parent == NULL) { //root case
        mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(m_GraphicObject);
        widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddViewProp(m_Assembly);
    }
}

void mafSceneNodeVTK::setVisibility(bool visible) {
    mafSceneNode::setVisibility(visible);
    
    if(visible) {
        mafProxy<vtkProp3D> *prop = mafProxyPointerTypeCast(vtkProp3D, visualPipe()->output());
        if(!m_Assembly->GetParts()->IsItemPresent(*prop)) {
            m_Assembly->AddPart(*prop);
        }
        update();
        visualPipe()->render();
    }
}

void mafSceneNodeVTK::update() {
    mafDataSetCollection *dc = vme()->dataSetCollection();
    mafMatrix *matrix = dc->poseMatrix();
    vtkMatrix4x4 *vtkMatrix = vtkMatrix4x4::New();
    vtkMatrix->DeepCopy(matrix->rawData());
    
    //vtkMatrix->PrintSelf(std::cout, vtkIndent(2));
    
    m_Assembly->SetUserMatrix(vtkMatrix);
    m_Assembly->Modified();
    vtkMatrix->Delete();
}
