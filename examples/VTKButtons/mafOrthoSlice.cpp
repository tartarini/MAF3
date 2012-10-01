/*
 *  mafOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 09/12/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOrthoSlice.h"
#include "ui_mafOrthoSlice.h"

#include <QDebug>

mafOrthoSlice::mafOrthoSlice(QWidget *parent) : QWidget(parent), ui(new Ui::mafOrthoSlice), m_Decimals(2) {
    ui->setupUi(this);
    ui->xPosition->setEnabled(false);
    ui->yPosition->setEnabled(false);
    ui->zPosition->setEnabled(false);
    
    
    ui->xLabel->setText("<font color='red'>X</font>");
    ui->yLabel->setText("<font color='green'>Y</font>");
    ui->zLabel->setText("<font color='blue'>Z</font>");
    
    m_Bounds[0] = 0.;
    m_Bounds[1] = 1.;
    m_Bounds[2] = 0.;
    m_Bounds[3] = 1.;
    m_Bounds[4] = 0.;
    m_Bounds[5] = 1.;
    m_Position[0] = m_Position[1] = m_Position[2] = 0.;
}

mafOrthoSlice::~mafOrthoSlice() {
    delete ui;
}

void mafOrthoSlice::setBounds(double bounds[6]) {
    double scaledBound;
    int sliderBounds[6];
    int multiplier;
    for (int i = 0; i < 6; ++i) {
        multiplier = 1;
        m_Bounds[i] = bounds[i];
        scaledBound = bounds[i];
        for (int d = 0; d < m_Decimals; ++d) {
            multiplier *= 10;
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

    m_Position[0] = (m_Bounds[0] + m_Bounds[1]) / 2;
    m_Position[1] = (m_Bounds[2] + m_Bounds[3]) / 2;
    m_Position[2] = (m_Bounds[4] + m_Bounds[5]) / 2;

    ui->xPosition->setValue((sliderBounds[0] + sliderBounds[1]) / 2);
    ui->yPosition->setValue((sliderBounds[2] + sliderBounds[3]) / 2);
    ui->zPosition->setValue((sliderBounds[4] + sliderBounds[5]) / 2);

    ui->xPosition->setTickInterval((sliderBounds[1] - sliderBounds[0]) / multiplier);
    ui->yPosition->setTickInterval((sliderBounds[3] - sliderBounds[2]) / multiplier);
    ui->zPosition->setTickInterval((sliderBounds[5] - sliderBounds[4]) / multiplier);
}

void mafOrthoSlice::setPosition(double pos[3]) {
    m_Position[0] = pos[0];
    m_Position[1] = pos[1];
    m_Position[2] = pos[2];
    double scaledVal[3];
    scaledVal[0] = pos[0];
    scaledVal[1] = pos[1];
    scaledVal[2] = pos[2];
    for (int d = 0; d < m_Decimals; ++d) {
        scaledVal[0] *= 10.;
        scaledVal[1] *= 10.;
        scaledVal[2] *= 10.;
    }
    ui->xPosition->setValue((int)scaledVal[0]);
    ui->yPosition->setValue((int)scaledVal[1]);
    ui->zPosition->setValue((int)scaledVal[2]);
}

void mafOrthoSlice::setDecimalDigits(int decimal) {
    m_Decimals = decimal;

    // Update ranges and values of position sliders.
    this->setBounds(m_Bounds);
}

void mafOrthoSlice::on_xPosition_sliderMoved(int val) {
    m_Position[0] = this->calculateRealValue(val);
    Q_EMIT positionUpdated(m_Position);
}

void mafOrthoSlice::on_yPosition_sliderMoved(int val) {
    m_Position[1] = this->calculateRealValue(val);
    Q_EMIT positionUpdated(m_Position);
}

void mafOrthoSlice::on_zPosition_sliderMoved(int val) {
    m_Position[2] = this->calculateRealValue(val);
    Q_EMIT positionUpdated(m_Position);
}

double mafOrthoSlice::calculateRealValue(int val) {
    double scaledVal = val;
    for (int d = 0; d < m_Decimals; ++d) {
        scaledVal /= 10.;
    }
    return scaledVal;
}

int mafOrthoSlice::calculateSliderValue(double val) {
    double scaledVal = val;
    int sliderVal;
    for (int d = 0; d < m_Decimals; ++d) {
        scaledVal *= 10;
    }
    sliderVal = (int)scaledVal;
    return sliderVal;
}
