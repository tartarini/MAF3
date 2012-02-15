/*
 *  mafBounds.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafBounds.h"

using namespace mafCore;

mafBounds::mafBounds(const QString code_location) : mafReferenceCounted(), m_XMin(0), m_YMin(0), m_ZMin(0), m_XMax(-1), m_YMax(-1), m_ZMax(-1) {
}

// mafBounds::mafBounds(const mafBounds &p) : m_XMin(p.xMin()), m_YMin(p.yMin()), m_ZMin(p.zMin()), m_XMax(p.xMax()), m_YMax(p.yMax()), m_ZMax(p.zMax()) {
// }

mafBounds::mafBounds(double pos[6], const QString code_location) : m_XMin(pos[0]), m_XMax(pos[1]), m_YMin(pos[2]), m_YMax(pos[3]), m_ZMin(pos[4]), m_ZMax(pos[5]) {
}

mafBounds::~mafBounds() {
}

mafBounds &mafBounds::operator =(const mafBounds& obj) {
    m_XMin = obj.xMin();
    m_XMax = obj.xMax();
    m_YMin = obj.yMin();
    m_YMax = obj.yMax();
    m_ZMin = obj.zMin();
    m_ZMax = obj.zMax();
    return *this;
}

bool mafBounds::isValid() {
	return m_XMin <= m_XMax && m_YMin <= m_YMax && m_ZMin <= m_ZMax;
}

void mafBounds::unite(const mafBounds &b) {
}

void mafBounds::intersect(const mafBounds &b) {
}

bool mafBounds::isPointInBounds(mafPoint *p) {
    return p->x() >= m_XMin && p->x <= m_XMax &&
        p->y() >= m_YMin && p->y() <= m_YMax &&
        p->z() >= m_ZMin && p->z() <= m_ZMax;
}
