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
#include "mafMatrix4x4.h"

#include <QGenericMatrix>

using namespace mafCore;
using namespace mafResources;

mafBounds::mafBounds(const QString code_location) : mafCore::mafReferenceCounted(), m_XMin(0), m_YMin(0), m_ZMin(0), m_XMax(-1), m_YMax(-1), m_ZMax(-1) {
    m_TransformationMatrix = new mafMatrix4x4();
    m_TransformationMatrix->setToIdentity();
}

// mafBounds::mafBounds(const mafBounds &p) : m_XMin(p.xMin()), m_YMin(p.yMin()), m_ZMin(p.zMin()), m_XMax(p.xMax()), m_YMax(p.yMax()), m_ZMax(p.zMax()) {
// }

mafBounds::mafBounds(double b[6], const QString code_location) : m_XMin(b[0]), m_XMax(b[1]), m_YMin(b[2]), m_YMax(b[3]), m_ZMin(b[4]), m_ZMax(b[5]) {
    m_TransformationMatrix = new mafMatrix4x4();
    m_TransformationMatrix->setToIdentity();
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
    m_TransformationMatrix = new mafMatrix4x4(*(obj.transformation()));
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
    
    m_TransformationMatrix->setToIdentity();
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
    this->transformation()->setToIdentity();
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
    m_TransformationMatrix->setToIdentity();
}

void mafBounds::setBounds(mafBounds *b) {
    m_XMin = b->xMin(); 
    m_XMax = b->xMax();
    m_YMin = b->yMin();
    m_YMax = b->yMax();
    m_ZMin = b->zMin();
    m_ZMax = b->zMax();
    delete m_TransformationMatrix;
    m_TransformationMatrix = new mafMatrix4x4(*(b->transformation()));
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

void mafBounds::transformBounds(mafMatrix4x4 *matrix) {
    if (matrix == NULL) {
        return;
    }

    (*m_TransformationMatrix) = (*matrix) * (*m_TransformationMatrix);
    
    QGenericMatrix<8,4, double> boundsMatrix;
 
    boundsMatrix(0,0) = m_XMin;
    boundsMatrix(1,0) =  m_YMin;
    boundsMatrix(2,0) =  m_ZMin;
    boundsMatrix(3,0) =  1.;

    
    boundsMatrix(0,1) = m_XMax;
    boundsMatrix(1,1) = m_YMin;
    boundsMatrix(2,1) = m_ZMin;
    boundsMatrix(3,1) = 1.;
    

    boundsMatrix(0,2) = m_XMax;
    boundsMatrix(1,2) = m_YMax;
    boundsMatrix(2,2) = m_ZMin;
    boundsMatrix(3,2) = 1.;


    boundsMatrix(0,3) = m_XMin;
    boundsMatrix(1,3) = m_YMax;
    boundsMatrix(2,3) = m_ZMin;
    boundsMatrix(3,3) = 1.;

    boundsMatrix(0,4) = m_XMin;
    boundsMatrix(1,4) = m_YMin;
    boundsMatrix(2,4) = m_ZMax;
    boundsMatrix(3,4) = 1.;

    boundsMatrix(0,5) = m_XMax;
    boundsMatrix(1,5) = m_YMin;
    boundsMatrix(2,5) = m_ZMax;
    boundsMatrix(3,5) = 1.;

    boundsMatrix(0,6) = m_XMax;
    boundsMatrix(1,6) = m_YMax;
    boundsMatrix(2,6) = m_ZMax;
    boundsMatrix(3,6) = 1.;
    
    boundsMatrix(0,7) = m_XMin;
    boundsMatrix(1,7) = m_YMax;
    boundsMatrix(2,7) = m_ZMax;
    boundsMatrix(3,7) = 1.;


    QGenericMatrix<8,4, qreal> transformedBounds = (*matrix) * boundsMatrix; // check this! before was inverted but generic multiplication (reverted again...since generic matrix takes as first the number of columns!)

    m_XMax = m_XMin = transformedBounds(0,0);
    m_YMax = m_YMin = transformedBounds(1,0);
    m_ZMax = m_ZMin = transformedBounds(2,0);
    double x, y, z;
    for (int i = 1; i < 8; ++i) {
        x = transformedBounds(0,i);
        m_XMin = x < m_XMin ? x : m_XMin;
        m_XMax = x > m_XMax ? x : m_XMax;
        y = transformedBounds(1,i);
        m_YMin = y < m_YMin ? y : m_YMin;
        m_YMax = y > m_YMax ? y : m_YMax;
        z = transformedBounds(2,i);
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
    qDebug() <<  m_TransformationMatrix;
}
