/*
 *  mafPipeVisualVTKSlice.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 17/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSlice.h"

#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <mafEventBusManager.h>


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafEventBus;

mafPipeVisualVTKSlice::mafPipeVisualVTKSlice(const QString code_location) : mafPipeVisualVTK(code_location), m_SliderPosition(NULL), m_PositionValue(NULL) {
    m_UIFilename = "mafPipeVisualSlice.ui";
    m_Origin = NULL;
    m_Normal = NULL;
    m_TransformMatrix = new mafMatrix();
    m_TransformMatrix->setIdentity();
}

mafPipeVisualVTKSlice::~mafPipeVisualVTKSlice() {
    mafDEL(m_Origin);
    mafDEL(m_Normal);
    if(m_TransformMatrix) {
        delete m_TransformMatrix;
    }
}

mafResources::mafPointPointer mafPipeVisualVTKSlice::origin() {
    //////////////////////////////////////////////////////////////////////////
    QString sig("origin()");
    mafCore::mafDelegatePointer delegateObj = delegateObject();
    if (delegateObj && delegateObj->isMethodDefined(sig)) {
        mafResources::mafPointPointer ret;
        QGenericReturnArgument ret_val = mafReturnArgument(mafResources::mafPointPointer, ret);
        delegateObj->executeMethod(sig, NULL, &ret_val);
        if (!delegateObj->shouldExecuteLocalCode()) {
            return ret;
        }
    }
    //////////////////////////////////////////////////////////////////////////

    return m_Origin;
}

mafResources::mafPointPointer mafPipeVisualVTKSlice::normal() {
    //////////////////////////////////////////////////////////////////////////
    QString sig("normal()");
    mafCore::mafDelegatePointer delegateObj = delegateObject();
    if (delegateObj && delegateObj->isMethodDefined(sig)) {
        mafResources::mafPointPointer ret;
        QGenericReturnArgument ret_val = mafReturnArgument(mafResources::mafPointPointer, ret);
        delegateObj->executeMethod(sig, NULL, &ret_val);
        if (!delegateObj->shouldExecuteLocalCode()) {
            return ret;
        }
    }
    //////////////////////////////////////////////////////////////////////////

    return m_Normal;
}

mafMatrixPointer mafPipeVisualVTKSlice::transformMatrix() {
    //////////////////////////////////////////////////////////////////////////
    QString sig("transformMatrix()");
    mafCore::mafDelegatePointer delegateObj = delegateObject();
    if (delegateObj && delegateObj->isMethodDefined(sig)) {
        mafResources::mafMatrixPointer ret;
        QGenericReturnArgument ret_val = mafReturnArgument(mafResources::mafMatrixPointer, ret);
        delegateObj->executeMethod(sig, NULL, &ret_val);
        if (!delegateObj->shouldExecuteLocalCode()) {
            return ret;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    // Request of ABS matrix should be optimized using a cached matrix that updates
    // only when a matrix changes on the hierarchy, otherwise returned the last 
    // abs matrix calculated.
    mafMatrixPointer absMatrix = NULL;
    mafObjectBase *obj = input();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrixPointer, absMatrix);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);
    m_TransformMatrix = absMatrix;

    return m_TransformMatrix;
}

void mafPipeVisualVTKSlice::updatePipe(double t) {
    Superclass::updatePipe(t);

    mafMatrix *tMatrix = transformMatrix();
    
    double b[6];
    mafVME *vme = input();
    vme->bounds(b, t);
    mafBounds *bounds = new mafBounds(b);
    bounds->transformBounds(tMatrix);
    bounds->bounds(b);
    
    //To use double values with QSlider, creates a number of steps in function
    //of range of Z bounds
    m_Range[0] = 0;
    m_Range[1] = (b[5]-b[4])*1000; 
    
    if (m_SliderPosition == NULL && m_PositionValue == NULL){
        m_SliderPosition = m_Range[1] / 2.;
        m_PositionValue = (b[5]-b[4])*(m_SliderPosition/m_Range[1])+b[4];
    }
    
    if (m_Origin == NULL) {
        m_Origin = new mafResources::mafPoint(/*(b[0] + b[1]) / 2., (b[2] + b[3]) / 2., (b[4] + b[5]) / 2.*/);
        vme->bounds()->center(*m_Origin);
    }
    
    if (m_Normal == NULL) {
        m_Normal = new mafResources::mafPoint(0., 0., 1.);
        
        if(tMatrix) {
            m_Normal->transformPoint(tMatrix);
        }
    }
}

void mafPipeVisualVTKSlice::setSlice(mafResources::mafPointPointer o) {
    *m_Origin = *o;
}

void mafPipeVisualVTKSlice::setNormal(mafResources::mafPointPointer n) {
    *m_Normal = *n;
}

void mafPipeVisualVTKSlice::setTransformMatrix(mafResources::mafMatrixPointer mat) {
    *m_TransformMatrix = *mat;
}

QString mafPipeVisualVTKSlice::positionValue() {
    return QString::number(m_PositionValue);
}

void mafPipeVisualVTKSlice::setPositionValue(QString value) {
    m_PositionValue = value.toDouble();
}

void mafPipeVisualVTKSlice::on_positionValue_textEdited(QString stringValue) {
    m_PositionValue = stringValue.toDouble();
}

void mafPipeVisualVTKSlice::on_positionValue_editingFinished() {
    double xMin,yMin, zMax, zMin;
    mafVME *vme = input();
    xMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->xMin();
    yMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->yMin();
    zMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->zMin();
    zMax = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->zMax();

    m_SliderPosition = (m_PositionValue-zMin)/(zMax-zMin)*m_Range[1];
    mafPoint *originPoint = new mafPoint(xMin, yMin, m_PositionValue);
    
    setSlice(originPoint);
    updateUI(widget());
    updatePipe();

    updateUI(widget());
    updatePipe();
}

void mafPipeVisualVTKSlice::on_positionValueSlider_sliderMoved(int value) {
    m_SliderPosition = value;

    double xMin,yMin, zMax, zMin;
    mafVME *vme = input();
    xMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->xMin();
    yMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->yMin();
    zMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->zMin();
    zMax = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->zMax();

    m_PositionValue = (zMax-zMin)*(m_SliderPosition/m_Range[1])+zMin;
    mafPoint *originPoint = new mafPoint(xMin, yMin, m_PositionValue);
    setSlice(originPoint);
    updatePipe();
    updateUI(widget());
}
