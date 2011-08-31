/*
 *  mafOperationTransform.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 17/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationTransform.h"
#include "mafDataSet.h"
#include "mafMatrix.h"
#include "mafVME.h"
#include "mafDataSetCollection.h"

using namespace mafResources;

mafOperationTransform::mafOperationTransform(const QString code_location) : mafOperation(code_location), m_DataSetCollection(NULL), m_Matrix(NULL), m_OldMatrix(NULL) {
    m_UIFilename = "mafOperationTransform.ui";
    setObjectName("mafOperationTransform");
}

mafOperationTransform::~mafOperationTransform() {
    if (m_Matrix){
        delete m_Matrix;
    }
    if (m_OldMatrix){
        delete m_OldMatrix;
    }
}

bool mafOperationTransform::acceptObject(mafCore::mafObjectBase *obj) {
    return true;
}

bool mafOperationTransform::initialize() {
    mafVME *inVME = qobject_cast<mafVME*>(input());
    m_DataSetCollection = inVME->dataSetCollection();
    m_OldMatrix = m_DataSetCollection->poseMatrix()->clone();
    m_OldMatrix->description();

    return Superclass::initialize();
}

QString mafOperationTransform::xPos() const {
    double pos[3];
    m_DataSetCollection->position(pos);
    return QString::number(pos[0]);
}

QString mafOperationTransform::yPos() const {
    double pos[3];
    m_DataSetCollection->position(pos);
    return QString::number(pos[1]);
}

QString mafOperationTransform::zPos() const {
    double pos[3];
    m_DataSetCollection->position(pos);
    return QString::number(pos[2]);
}

void mafOperationTransform::setXPos(const QString x) {
    double pos[3];
    m_DataSetCollection->position(pos);
    bool ok;
    pos[0] = x.toDouble(&ok);
    if (ok) {
        m_DataSetCollection->setPosition(pos);
    }
}

void mafOperationTransform::setYPos(const QString y) {
    double pos[3];
    m_DataSetCollection->position(pos);
    bool ok;
    pos[1] = y.toDouble(&ok);
    if (ok) {
        m_DataSetCollection->setPosition(pos);
    }
}

void mafOperationTransform::setZPos(const QString z) {
    double pos[3];
    m_DataSetCollection->position(pos);
    bool ok;
    pos[2] = z.toDouble(&ok);
    if (ok) {
        m_DataSetCollection->setPosition(pos);
    }
}

QString mafOperationTransform::xRot() const {
    double ori[3];
    m_DataSetCollection->orientations(ori);
    return QString::number(ori[0]);
}

QString mafOperationTransform::yRot() const {
    double ori[3];
    m_DataSetCollection->orientations(ori);
    return QString::number(ori[1]);
}

QString mafOperationTransform::zRot() const {
    double ori[3];
    m_DataSetCollection->orientations(ori);
    return QString::number(ori[2]);
}

void mafOperationTransform::setXRot(const QString xrot) {
    double ori[3];
    m_DataSetCollection->orientations(ori);
    bool ok;
    ori[0] = xrot.toDouble(&ok);
    if (ok) {
        m_DataSetCollection->setOrientation(ori[0], ori[1], ori[2]);
    }
}

void mafOperationTransform::setYRot(const QString yrot) {
    double ori[3];
    m_DataSetCollection->orientations(ori);
    bool ok;
    ori[1] = yrot.toDouble(&ok);
    if (ok) {
        m_DataSetCollection->setOrientation(ori[0], ori[1], ori[2]);
    }
}

void mafOperationTransform::setZRot(const QString zrot) {
    double ori[3];
    m_DataSetCollection->orientations(ori);
    bool ok;
    ori[2] = zrot.toDouble(&ok);
    if (ok) {
        m_DataSetCollection->setOrientation(ori[0], ori[1], ori[2]);
    }
}

void mafOperationTransform::execute() {
    m_Status = mafOperationStatusExecuting;
    m_Matrix = m_DataSetCollection->poseMatrix()->clone();
    m_Matrix->description();
    qDebug() << mafTr("Executing transform operation...");
    emit executionEnded();
}

void mafOperationTransform::unDo() {
    m_DataSetCollection->setPose(*m_OldMatrix);
}

void mafOperationTransform::reDo() {
    m_DataSetCollection->setPose(*m_Matrix);
}

void mafOperationTransform::terminated() {
    if(m_Status == mafOperationStatusCanceled) {
        unDo();
    }
    
    qDebug() << mafTr("Terminated with status: ") << m_Status;
}

/// ui binding
void mafOperationTransform::on_xPos_textEdited(const QString &text) {
    setXPos(text);
}
void mafOperationTransform::on_yPos_textEdited(const QString &text) {
    setYPos(text);
}
void mafOperationTransform::on_zPos_textEdited(const QString &text) {
    setZPos(text);
}

void mafOperationTransform::on_xRot_textEdited(const QString &text) {
    setXRot(text);
}

void mafOperationTransform::on_yRot_textEdited(const QString &text) {
    setYRot(text);
}
void mafOperationTransform::on_zRot_textEdited(const QString &text) {
    setZRot(text);
}

