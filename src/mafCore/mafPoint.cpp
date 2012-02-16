/*
 *  mafPoint.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPoint.h"

using namespace mafCore;

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

