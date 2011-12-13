/*
 *  mafViewOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewOrthoSlice.h"
#include "mafOrthoSlice.h"
#include <mafVME.h>

mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafResources::mafViewCompound(code_location) {
	setConfigurationFile("OrthoSlice.xml");
    m_GUI = new mafOrthoSlice();
    this->setUIRootWidget(m_GUI);
    connect(m_GUI, SIGNAL(positionUpdated(double *)), this, SLOT(sliceAtPosition(double *)));
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
}

void mafViewOrthoSlice::sliceAtPosition(double *pos) {
    m_SlicePosition[0] = pos[0];
    m_SlicePosition[1] = pos[1];
    m_SlicePosition[2] = pos[2];
}

void mafViewOrthoSlice::showSceneNode(mafResources::mafSceneNode *node, bool show) {
    Superclass::showSceneNode(node, show);
    if (show) {
        mafCore::mafObjectBase *vp = (mafCore::mafObjectBase *)node->visualPipe();
//        vp->setDelegateObject(this);
        mafResources::mafVME *vme = node->vme();
        double b[6];
        vme->bounds(b);
        m_GUI->setBounds(b);
    }
}

//////////////////////////////////////////////////////////////////////////
// Delegate methods
//////////////////////////////////////////////////////////////////////////
void mafViewOrthoSlice::setOriginX(QString stringValue) {

}

void mafViewOrthoSlice::setOriginY(QString stringValue) {

}

void mafViewOrthoSlice::setOriginZ(QString stringValue) {

}
