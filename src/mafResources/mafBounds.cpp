/*
 *  mafBounds.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafBounds.h"
#include "mafMatrix.h"

using namespace mafCore;
using namespace mafResources;

mafBounds::mafBounds(const QString code_location) : mafCore::mafReferenceCounted(), m_XMin(0), m_YMin(0), m_ZMin(0), m_XMax(-1), m_YMax(-1), m_ZMax(-1) {
    m_TransformationMatrix = new mafMatrix(4,4);
    m_TransformationMatrix->setIdentity();
}

// mafBounds::mafBounds(const mafBounds &p) : m_XMin(p.xMin()), m_YMin(p.yMin()), m_ZMin(p.zMin()), m_XMax(p.xMax()), m_YMax(p.yMax()), m_ZMax(p.zMax()) {
// }

mafBounds::mafBounds(double b[6], const QString code_location) : m_XMin(b[0]), m_XMax(b[1]), m_YMin(b[2]), m_YMax(b[3]), m_ZMin(b[4]), m_ZMax(b[5]) {
    m_TransformationMatrix = new mafMatrix(4,4);
    m_TransformationMatrix->setIdentity();
}

mafBounds::~mafBounds() {
    delete m_TransformationMatrix;
}

mafBounds &mafBounds::operator =(const mafBounds& obj) {
    m_XMin = obj.xMin();
    m_XMax = obj.xMax();
    m_YMin = obj.yMin();
    m_YMax = obj.yMax();
    m_ZMin = obj.zMin();
    m_ZMax = obj.zMax();
    m_TransformationMatrix = obj.transformation()->clone();
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

void mafBounds::unite(const mafBounds &b) {
    double bounds[6];
    bounds[0] = MIN(this->xMin(), b.xMin());
    bounds[1] = MAX(this->xMax(), b.xMax());
    
    bounds[2] = MIN(this->yMin(), b.yMin());
    bounds[3] = MAX(this->yMax(), b.yMax());
    
    bounds[4] = MIN(this->zMin(), b.zMin());
    bounds[5] = MAX(this->zMax(), b.zMax());
    
    this->setBounds(bounds);
    
    m_TransformationMatrix->setIdentity();
}

void mafBounds::intersect(const mafBounds &b) {
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
    
    this->setBounds(outputBound);
    this->transformation()->setIdentity();
}

void mafBounds::bounds(double *b) {
    REQUIRE(b);
    b[0] = m_XMin; 
    b[1] = m_XMax;
    b[2] = m_YMin;
    b[3] = m_YMax;
    b[4] = m_ZMin;
    b[5] = m_ZMax;
}

void mafBounds::setBounds(double b[6]) {
    m_XMin = b[0]; 
    m_XMax = b[1];
    m_YMin = b[2];
    m_YMax = b[3];
    m_ZMin = b[4];
    m_ZMax = b[5];
    m_TransformationMatrix->setIdentity();
}

void mafBounds::setBounds(mafBounds *b) {
    m_XMin = b->xMin(); 
    m_XMax = b->xMax();
    m_YMin = b->yMin();
    m_YMax = b->yMax();
    m_ZMin = b->zMin();
    m_ZMax = b->zMax();
    delete m_TransformationMatrix;
    m_TransformationMatrix = b->transformation()->clone();
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
    c[0] = (m_XMin + m_XMax) * .5;
    c[1] = (m_YMin + m_YMax) * .5;
    c[2] = (m_ZMin + m_ZMax) * .5;
}

void mafBounds::center(mafPoint &p) {
    double c[3];
    center(c);
    p.setPosition(c);
}

bool mafBounds::isPointInBounds(mafPoint *p) {
    return p->x() >= m_XMin && p->x() <= m_XMax &&
        p->y() >= m_YMin && p->y() <= m_YMax &&
        p->z() >= m_ZMin && p->z() <= m_ZMax;
}

void mafBounds::transformBounds(mafMatrix *matrix) {
    if (matrix == NULL) {
        return;
    }

    (*m_TransformationMatrix) = (*matrix) * (*m_TransformationMatrix);
    
    mafMatrix boundsMatrix(4, 8);

    mafMatrix p0(4,1);
    p0.setElement(0,0, m_XMin);
    p0.setElement(1,0, m_YMin);
    p0.setElement(2,0, m_ZMin);
    p0.setElement(3,0, 1.);
    boundsMatrix.setColumn(p0, 0);

    mafMatrix p1(4,1);
    p1.setElement(0,0, m_XMax);
    p1.setElement(1,0, m_YMin);
    p1.setElement(2,0, m_ZMin);
    p1.setElement(3,0, 1.);
    boundsMatrix.setColumn(p1, 1);

    mafMatrix p2(4,1);
    p2.setElement(0,0, m_XMax);
    p2.setElement(1,0, m_YMax);
    p2.setElement(2,0, m_ZMin);
    p2.setElement(3,0, 1.);
    boundsMatrix.setColumn(p2, 2);

    mafMatrix p3(4,1);
    p3.setElement(0,0, m_XMin);
    p3.setElement(1,0, m_YMax);
    p3.setElement(2,0, m_ZMin);
    p3.setElement(3,0, 1.);
    boundsMatrix.setColumn(p3, 3);

    mafMatrix p4(4,1);
    p4.setElement(0,0, m_XMin);
    p4.setElement(1,0, m_YMin);
    p4.setElement(2,0, m_ZMax);
    p4.setElement(3,0, 1.);
    boundsMatrix.setColumn(p4, 4);

    mafMatrix p5(4,1);
    p5.setElement(0,0, m_XMax);
    p5.setElement(1,0, m_YMin);
    p5.setElement(2,0, m_ZMax);
    p5.setElement(3,0, 1.);
    boundsMatrix.setColumn(p5, 5);

    mafMatrix p6(4,1);
    p6.setElement(0,0, m_XMax);
    p6.setElement(1,0, m_YMax);
    p6.setElement(2,0, m_ZMax);
    p6.setElement(3,0, 1.);
    boundsMatrix.setColumn(p6, 6);

    mafMatrix p7(4,1);
    p7.setElement(0,0, m_XMin);
    p7.setElement(1,0, m_YMax);
    p7.setElement(2,0, m_ZMax);
    p7.setElement(3,0, 1.);
    boundsMatrix.setColumn(p7, 7);

    mafMatrix transformedBounds = (*matrix) * boundsMatrix;

    m_XMax = m_XMin = transformedBounds.element(0, 0);
    m_YMax = m_YMin = transformedBounds.element(1, 0);
    m_ZMax = m_ZMin = transformedBounds.element(2, 0);
    double x, y, z;
    for (int i = 1; i < 8; ++i) {
        x = transformedBounds.element(0, i);
        m_XMin = x < m_XMin ? x : m_XMin;
        m_XMax = x > m_XMax ? x : m_XMax;
        y = transformedBounds.element(1, i);
        m_YMin = y < m_YMin ? y : m_YMin;
        m_YMax = y > m_YMax ? y : m_YMax;
        z = transformedBounds.element(2, i);
        m_ZMin = z < m_ZMin ? z : m_ZMin;
        m_ZMax = z > m_ZMax ? z : m_ZMax;
    }
}

void mafBounds::description() const {
    // Print the information contained into the class.
    qDebug() << "X Min: " << m_XMin << " X Max: " << m_XMax;
    qDebug() << "Y Min: " << m_YMin << " Y Max: " << m_YMax;
    qDebug() << "Z Min: " << m_ZMin << " Z Max: " << m_ZMax;
    
    qDebug() << "Transform Matrix: ";
    m_TransformationMatrix->description();
}
