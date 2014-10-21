/*
 *  mafPoint.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
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


void mafPoint::transformPoint(mafMatrix4x4 *matrix) {
    QGenericMatrix<1,4,qreal> p0;
    p0(0,0) = m_X;
    p0(1,0) = m_Y;
    p0(2,0) = m_Z;
    p0(3,0) = 1.;

    QGenericMatrix<1,4,qreal> pt0 = (*matrix) * p0; // inverted when changed matrix base class
    m_X = pt0(0,0);
    m_Y = pt0(1,0);
    m_Z = pt0(2,0);
}
