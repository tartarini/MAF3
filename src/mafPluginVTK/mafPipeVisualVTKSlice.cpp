/*
 *  mafPipeVisualVTKSlice.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 17/02/12.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSlice.h"

#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


mafPipeVisualVTKSlice::mafPipeVisualVTKSlice(const QString code_location) : mafPipeVisualVTK(code_location), m_PositionValue(NULL) {
    m_UIFilename = "mafPipeVisualSlice.ui";
    m_Origin = NULL;
    m_Normal = NULL;
}

mafPipeVisualVTKSlice::~mafPipeVisualVTKSlice() {
    mafDEL(m_Origin);
    mafDEL(m_Normal);
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

void mafPipeVisualVTKSlice::updatePipe(double t) {
    Superclass::updatePipe(t);

    double b[6];
    mafVME *vme = input();
    vme->bounds(b, t);

    //m_Range[0] = b[4];
    //m_Range[1] = b[5];

    //set m_Range form 0 to maximum step of QSlider 
    int steps = (abs(b[4]) + abs(b[5]))*100;
    m_Range[0] = 0;
    m_Range[1] = steps*100; 

    if (m_PositionValue == NULL){
        m_PositionValue = m_Range[1] / 2.;
    }
    
    if (m_Origin == NULL) {
        m_Origin = new mafResources::mafPoint((b[0] + b[1]) / 2., (b[2] + b[3]) / 2., (b[4] + b[5]) / 2.);
    }
    if (m_Normal == NULL) {
        m_Normal = new mafResources::mafPoint(0., 0., 1.);
    }
}

void mafPipeVisualVTKSlice::setSlice(mafResources::mafPointPointer o) {
    *m_Origin = *o;
}

void mafPipeVisualVTKSlice::setNormal(mafResources::mafPointPointer n) {
    *m_Normal = *n;
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
    double xMin;
    double yMin;
    mafVME *vme = input();
    xMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->xMin();
    yMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->yMin();

    mafPoint *originPoint = new mafPoint(xMin, yMin, m_PositionValue);
    setSlice(originPoint);
    updateUI(widget());
    updatePipe();

    updateUI(widget());
    updatePipe();
}

void mafPipeVisualVTKSlice::on_positionValueSlider_sliderMoved(int value) {
    m_PositionValue = value;

    double xMin;
    double yMin;
    double zMax, zMin;
    mafVME *vme = input();
    xMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->xMin();
    yMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->yMin();
    zMin = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->zMin();
    zMax = vme->dataSetCollection()->itemAtCurrentTime()->bounds()->zMax();

    //double slicePos = (zMin+zMax)*(value/m_Range[1])+zMin;
    double slicePos = (zMax-zMin)*(value/m_Range[1])+zMin;
    mafPoint *originPoint = new mafPoint(xMin, yMin, slicePos);
    setSlice(originPoint);
    updatePipe();
    updateUI(widget());
}
