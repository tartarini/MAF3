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

// mafPoint::mafPoint(const mafPoint &p) : m_X(p.m_X), m_Y(p.m_Y), m_Z(p.m_Z) {
// }

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
