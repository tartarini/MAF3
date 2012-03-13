/*
 *  mafPipeDataSlice.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 18/1/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSlice.h"
#include "mafVME.h"
#include "mafDataSet.h"

using namespace mafCore;
using namespace mafResources;


mafPipeDataSlice::mafPipeDataSlice(const QString code_location) : mafPipeData(code_location) {
    m_SliceOrigin[0] = m_SliceOrigin[1] = m_SliceOrigin[2] = 0.;

    this->setPlaneNormalAxes(mafPlaneNormalZ);
}

mafPipeDataSlice::~mafPipeDataSlice() {
}

void mafPipeDataSlice::updatePipe(double t) {
    Superclass::updatePipe(t);
}

void mafPipeDataSlice::pushSlice(double distance) {
    if (distance != 0) {
        for (int i=0; i < 3; ++i) {
            this->m_SliceOrigin[i] += distance * this->m_Normal[i];
        }
        this->updatePipe();
    }
}

void mafPipeDataSlice::setNormal(double *normal) {
    REQUIRE(normal);

    m_Normal[0] = normal[0];
    m_Normal[1] = normal[1];
    m_Normal[2] = normal[2];

    m_PlaneNormalAxes = mafPlaneNormalArbitrary;

/*    double res[3], n;
    double xyz[3][3] = {{1., 0., 0.}, {0., 1., 0.}, {0., 0., 1.}};
    for (int a = 0; a < 3; ++a) {
        vtkMath::Cross(m_Normal, xyz[a], res);
        n = vtkMath::Norm(res);
        if (n == 0) {
            m_PlaneNormalAxes = (mafPlaneNormal)a;
            break;
        }
    }*/
}

void mafPipeDataSlice::setPlaneNormalAxes(mafPlaneNormal axes) {
    m_PlaneNormalAxes = axes;
    switch (axes) {
        case mafPlaneNormalX:
            m_XVector[0] = 0.0001; //this is a ancestor bug... very sad.
            m_XVector[1] = 1.;
            m_XVector[2] = 0.;
            m_YVector[0] = 0.;
            m_YVector[1] = 0.;
            m_YVector[2] = 1.;
            m_Normal[0] = 1.;
            m_Normal[1] = 0.;
            m_Normal[2] = 0.;
            break;
        case mafPlaneNormalY:
            m_XVector[0] = 0.;
            m_XVector[1] = 0.;
            m_XVector[2] = 1.;
            m_YVector[0] = 1.;
            m_YVector[1] = 0.;
            m_YVector[2] = 0.;
            m_Normal[0] = 0.;
            m_Normal[1] = 1.;
            m_Normal[2] = 0.;
            break;
        case mafPlaneNormalZ:
            m_XVector[0] = 1.;
            m_XVector[1] = 0.;
            m_XVector[2] = 0.;
            m_YVector[0] = 0.;
            m_YVector[1] = 1.;
            m_YVector[2] = 0.;
            m_Normal[0] = 0.;
            m_Normal[1] = 0.;
            m_Normal[2] = 1.;
            break;
        default:
            qDebug() << mafTr("Arbitrary slicing...");
            break;
    }
}

void mafPipeDataSlice::setTransformMatrix(mafMatrix &m) {
    m_TransformMatrix = m;
    setModified();
}
