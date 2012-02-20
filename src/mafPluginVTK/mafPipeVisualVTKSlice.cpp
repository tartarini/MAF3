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

mafPipeVisualVTKSlice::mafPipeVisualVTKSlice(const QString code_location) : mafPipeVisualVTK(code_location) {
    m_Origin = NULL;
    m_Normal = NULL;
}

mafPipeVisualVTKSlice::~mafPipeVisualVTKSlice() {
    mafDEL(m_Origin);
    mafDEL(m_Normal);
}

mafCore::mafPointPointer mafPipeVisualVTKSlice::origin() {
    //////////////////////////////////////////////////////////////////////////
    QString sig("origin()");
    mafCore::mafDelegatePointer delegateObj = delegateObject();
    if (delegateObj && delegateObj->isMethodDefined(sig)) {
        mafCore::mafPointPointer ret;
        QGenericReturnArgument ret_val = mafReturnArgument(mafCore::mafPointPointer, ret);
        delegateObj->executeMethod(sig, NULL, &ret_val);
        if (!delegateObj->shouldExecuteLocalCode()) {
            return ret;
        }
    }
    //////////////////////////////////////////////////////////////////////////

    return m_Origin;
}

mafCore::mafPointPointer mafPipeVisualVTKSlice::normal() {
    //////////////////////////////////////////////////////////////////////////
    QString sig("normal()");
    mafCore::mafDelegatePointer delegateObj = delegateObject();
    if (delegateObj && delegateObj->isMethodDefined(sig)) {
        mafCore::mafPointPointer ret;
        QGenericReturnArgument ret_val = mafReturnArgument(mafCore::mafPointPointer, ret);
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
    
    if (m_Origin == NULL) {
        double b[6];
        mafVME *vme = input();
        vme->bounds(b, t);
        m_Origin = new mafCore::mafPoint((b[0] + b[1]) / 2., (b[2] + b[3]) / 2., (b[4] + b[5]) / 2.);
    }
    if (m_Normal == NULL) {
        m_Normal = new mafCore::mafPoint(0.,0., 1.);
    }
}

void mafPipeVisualVTKSlice::setSlice(mafCore::mafPointPointer o) {
    *m_Origin = *o;
}

void mafPipeVisualVTKSlice::setNormal(mafCore::mafPointPointer n) {
    *m_Normal = *n;
}
