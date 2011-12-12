/*
 *  mafOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 09/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOrthoSlice.h"
#include "ui_mafOrthoSlice.h"

#include <qDebug>

mafOrthoSlice::mafOrthoSlice(QWidget *parent) : QWidget(parent), ui(new Ui::mafOrthoSlice) {
    ui->setupUi(this);
    ui->xPosition->setEnabled(false);
    ui->yPosition->setEnabled(false);
    ui->zPosition->setEnabled(false);
    m_Bounds[0] = 0.;
    m_Bounds[1] = 1.;
    m_Bounds[2] = 0.;
    m_Bounds[3] = 1.;
    m_Bounds[4] = 0.;
    m_Bounds[5] = 1.;
}

mafOrthoSlice::~mafOrthoSlice() {
    delete ui;
}

void mafOrthoSlice::setBounds(double bounds[6]) {
    double scaledBound;
    int sliderBounds[6];
    for (int i = 0; i < 6; ++i) {
        m_Bounds[i] = bounds[i];
        scaledBound = bounds[i];
        for (int d = 0; d < m_Decimals; ++d) {
            scaledBound *= 10;
        }
        sliderBounds[i] = (int)scaledBound;
    }

    ui->xPosition->setEnabled(sliderBounds[0] < sliderBounds[1]);
    ui->yPosition->setEnabled(sliderBounds[2] < sliderBounds[3]);
    ui->zPosition->setEnabled(sliderBounds[4] < sliderBounds[5]);

    // update the slider's range and value according to the new bounds.
    ui->xPosition->setRange(sliderBounds[0], sliderBounds[1]);
    ui->yPosition->setRange(sliderBounds[2], sliderBounds[3]);
    ui->zPosition->setRange(sliderBounds[4], sliderBounds[5]);

    ui->xPosition->setValue((sliderBounds[0] + sliderBounds[1]) / 2);
    ui->yPosition->setValue((sliderBounds[2] + sliderBounds[3]) / 2);
    ui->zPosition->setValue((sliderBounds[4] + sliderBounds[5]) / 2);
}

void mafOrthoSlice::setDecimalDigits(int decimal) {
    m_Decimals = decimal;

    // Update ranges and values of position sliders.
    this->setBounds(m_Bounds);
}

void mafOrthoSlice::on_xPosition_sliderMoved(int val) {
    double scaledVal = this->calculateRealValue(val);
    qDebug() << QString("X: %1").arg(scaledVal);
}

void mafOrthoSlice::on_yPosition_sliderMoved(int val) {
    double scaledVal = this->calculateRealValue(val);
    qDebug() << QString("Y: %1").arg(scaledVal);
}

void mafOrthoSlice::on_zPosition_sliderMoved(int val) {
    double scaledVal = this->calculateRealValue(val);
    qDebug() << QString("Z: %1").arg(scaledVal);
}

double mafOrthoSlice::calculateRealValue(int val) {
    double scaledVal = val;
    for (int d = 0; d < m_Decimals; ++d) {
        scaledVal /= 10.;
    }
    return scaledVal;
}
