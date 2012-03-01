/*
 *  mafBounds.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafBounds.h"
#include "mafPoint.h"

using namespace mafCore;
using namespace mafResources;

mafBounds::mafBounds(const QString code_location) : mafCore::mafReferenceCounted(), m_XMin(0), m_YMin(0), m_ZMin(0), m_XMax(-1), m_YMax(-1), m_ZMax(-1) {
}

// mafBounds::mafBounds(const mafBounds &p) : m_XMin(p.xMin()), m_YMin(p.yMin()), m_ZMin(p.zMin()), m_XMax(p.xMax()), m_YMax(p.yMax()), m_ZMax(p.zMax()) {
// }

mafBounds::mafBounds(double b[6], const QString code_location) : m_XMin(b[0]), m_XMax(b[1]), m_YMin(b[2]), m_YMax(b[3]), m_ZMin(b[4]), m_ZMax(b[5]) {
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

bool mafBounds::operator==(const mafBounds &obj) const {
    bool res = (mafEquals(m_XMin,obj.xMin()) && mafEquals(m_YMin,obj.yMin()) && mafEquals(m_ZMin,obj.zMin()) &&
                mafEquals(m_XMax,obj.xMax()) && mafEquals(m_YMax,obj.yMax()) && mafEquals(m_ZMax,obj.zMax()));
    return res;
}

bool mafBounds::operator!=(const mafBounds &obj) const {
    return !(*this == obj);
}

bool mafBounds::isObjectValid() const {
	return m_XMin <= m_XMax && m_YMin <= m_YMax && m_ZMin <= m_ZMax;
}

void mafBounds::unite(const mafBounds &b, mafBounds &output) {
    double bounds[6];
    bounds[0] = MIN(this->xMin(), b.xMin());
    bounds[1] = MAX(this->xMax(), b.xMax());
    
    bounds[2] = MIN(this->yMin(), b.yMin());
    bounds[3] = MAX(this->yMax(), b.yMax());
    
    bounds[4] = MIN(this->zMin(), b.zMin());
    bounds[5] = MAX(this->zMax(), b.zMax());
    
    output.setBounds(bounds);
}

void mafBounds::intersect(const mafBounds &b, mafBounds &output) {
    mafBounds *a = this;
    /// Xmin check
    bool bXMinInsideA = b.xMin() >= a->xMin() && b.xMin() <= a->xMax();
    /// Xmax check
    bool bXMaxInsideA = b.xMax() >= a->xMin() && b.xMax() <= a->xMax();
 
    
    /// Ymin check
    bool bYMinInsideA = b.yMin() >= a->yMin() && b.yMin() <= a->yMax();
    /// Ymax check
    bool bYMaxInsideA = b.yMax() >= a->yMin() && b.yMax() <= a->yMax();
 
    
    /// Zmin check
    bool bZMinInsideA = b.zMin() >= a->zMin() && b.zMin() <= a->zMax();
    /// Zmax check 
    bool bZMaxInsideA = b.zMax() >= a->zMin() && b.zMax() <= a->zMax();
    
    
    bool XIntersection = (bXMinInsideA && !bXMaxInsideA) || (bXMinInsideA && bYMaxInsideA) || (!bXMinInsideA && bXMaxInsideA);
    bool YIntersection = (bYMinInsideA && !bYMaxInsideA) || (bYMinInsideA && bYMaxInsideA) || (!bYMinInsideA && bYMaxInsideA);
    bool ZIntersection = (bZMinInsideA && !bZMaxInsideA) || (bZMinInsideA && bZMaxInsideA) || (!bZMinInsideA && bZMaxInsideA);
    
    double outputBound[6] = {0.,-1,0.,-1,0.,-1};
    if(XIntersection && YIntersection && ZIntersection) {
        outputBound[0] = bXMinInsideA ? b.xMin() : a->xMin();
        outputBound[1] = bXMaxInsideA ? b.xMax() : a->xMax();
        
        outputBound[2] = bYMinInsideA ? b.yMin() : a->yMin();
        outputBound[3] = bYMaxInsideA ? b.yMax() : a->yMax();
        
        outputBound[4] = bZMinInsideA ? b.zMin() : a->zMin();
        outputBound[5] = bZMaxInsideA ? b.zMax() : a->zMax();
    }
    
    output.setBounds(outputBound);
}

void mafBounds::setBounds(double b[6]) {
    m_XMin = b[0]; 
    m_XMax = b[1];
    m_YMin = b[2];
    m_YMax = b[3];
    m_ZMin = b[4];
    m_ZMax = b[5];
}

void mafBounds::setBounds(mafBounds *b) {
    m_XMin = b->xMin(); 
    m_XMax = b->xMax();
    m_YMin = b->yMin();
    m_YMax = b->yMax();
    m_ZMin = b->zMin();
    m_ZMax = b->zMax();
}

double mafBounds::length() {
    double b_diff[3];
    double temp;
    temp = xMax()- xMin();
    b_diff[0] = temp * temp;
    temp = yMax()- yMin();
    b_diff[1] = temp * temp;
    temp = zMax()- zMin();
    b_diff[2] = temp * temp;
    
    return sqrtl(b_diff[0] + b_diff[1] + b_diff[2]);
}

void mafBounds::center(double c[3]) {
    c[0] = (xMin() + xMax()) * .5;
    c[1] = (yMin() + yMax()) * .5;
    c[2] = (zMin() + zMax()) * .5;
}

bool mafBounds::isPointInBounds(mafPoint *p) {
    return p->x() >= m_XMin && p->x() <= m_XMax &&
        p->y() >= m_YMin && p->y() <= m_YMax &&
        p->z() >= m_ZMin && p->z() <= m_ZMax;
}

void mafResources::mafBounds::transformBounds(mafMatrix *matrix) {
    //find center of the bounding box
    double oldCenter[3];
    oldCenter[0] = (xMin() + xMax()) / 2;
    oldCenter[1] = (yMin() + yMax()) / 2;
    oldCenter[2] = (zMin() + zMax()) / 2;

    //move the center to the origin
    double origin[3] = {0., 0., 0.};


    //apply the transform to the center
    mafPoint *originPoint = new mafPoint(origin);
    originPoint->transformPoint(matrix);

    //new transformed center
    double newOrigin[3];
    newOrigin[0] = originPoint->x();
    newOrigin[1] = originPoint->y();
    newOrigin[2] = originPoint->z();

    //extract the rotation sub-matrix and calculate the angles considering the Yaw-Pitch-Roll convention.
    double x0, x1, x2;
    double y0, y1, y2;
    double z0, z1, z2;

    x0 = matrix->element(0,0);
    x1 = matrix->element(1,0);
    x2 = matrix->element(2,0);
    y0 = matrix->element(0,1);
    y1 = matrix->element(1,1);
    y2 = matrix->element(2,1);
    z0 = matrix->element(0,2);
    z1 = matrix->element(1,2);
    z2 = matrix->element(2,2);

    //move back center to original center position
    newOrigin[0] = newOrigin[0] + oldCenter[0];
    newOrigin[1] = newOrigin[1] + oldCenter[1];
    newOrigin[2] = newOrigin[2] + oldCenter[2];

    double xOffset = (xMax()-xMin())/2;
    double yOffset = (yMax()-yMin())/2;
    double zOffset = (zMax()-zMin())/2;

    double newBounds[6];
    newBounds[0] = newOrigin[0] - ((xOffset*x0) + (xOffset*x1) + (xOffset*x2));
    newBounds[1] = newOrigin[0] + ((xOffset*x0) + (xOffset*x1) + (xOffset*x2));
    newBounds[2] = newOrigin[1] - ((yOffset*y0) + (yOffset*y1) + (yOffset*y2));
    newBounds[3] = newOrigin[1] + ((yOffset*y0) + (yOffset*y1) + (yOffset*y2));
    newBounds[4] = newOrigin[2] - ((zOffset*z0) + (zOffset*z1) + (zOffset*z2));
    newBounds[5] = newOrigin[2] + ((zOffset*z0) + (zOffset*z1) + (zOffset*z2));
    this->setBounds(newBounds);
}