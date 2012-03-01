/*
 *  mafPoint.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPoint.h"

using namespace mafCore;
using namespace mafResources;

mafPoint::mafPoint(const QString code_location) : mafReferenceCounted(), m_X(0), m_Y(0), m_Z(0) {
}

mafPoint::mafPoint(double x_pos, double y_pos, double z_pos) : m_X(x_pos), m_Y(y_pos), m_Z(z_pos) {

}

mafPoint::mafPoint(double pos[3], const QString code_location) : m_X(pos[0]), m_Y(pos[1]), m_Z(pos[2]) {
}

mafPoint::~mafPoint() {
}

mafPoint &mafPoint::operator =(const mafPoint& obj) {
    m_X = obj.x();
    m_Y = obj.y();
    m_Z = obj.z();
    return *this;
}

bool mafPoint::operator==(const mafPoint &obj) const {
    bool res = mafEquals(m_X, obj.x()) && mafEquals(m_Y, obj.y()) && mafEquals(m_Z, obj.z());
    return res;
}

bool mafPoint::operator!=(const mafPoint &obj) const {
    return !(*this == obj);
}

void mafPoint::pos(double p[3]) {
    p[0] = m_X;
    p[1] = m_Y;
    p[2] = m_Z;
}


void mafPoint::transformPoint(mafMatrix *matrix) {
    // Homogeneous transformation
    double x = matrix->element(0,0)*this->x() + matrix->element(0,1)*this->y() + matrix->element(0,2)*this->z() + matrix->element(0,3);
    double y = matrix->element(1,0)*this->x() + matrix->element(1,1)*this->y() + matrix->element(1,2)*this->z() + matrix->element(1,3);
    double z = matrix->element(2,0)*this->x() + matrix->element(2,1)*this->y() + matrix->element(2,2)*this->z() + matrix->element(2,3);
    double w = matrix->element(3,0)*this->x() + matrix->element(3,1)*this->y() + matrix->element(3,2)*this->z() + matrix->element(3,3);

    double f = 1.0/w;
    this->setX(x*f); 
    this->setY(y*f); 
    this->setZ(z*f);
}