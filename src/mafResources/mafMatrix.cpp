/*
 *  mafMatrix.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 13/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMatrix.h"

using namespace cv;
using namespace mafResources;

mafMatrix::mafMatrix(int rows, int cols) {
    m_Matrix = cvCreateMat(rows, cols, CV_64FC1);
}


mafMatrix::~mafMatrix() {
    cvReleaseMat(&m_Matrix);
}

mafMatrix & mafMatrix::operator =(const mafMatrix &mat) {
    cvCopy(mat.m_Matrix, m_Matrix);
    return *this;
}

mafMatrix *mafMatrix::clone() const{
    mafMatrix *m= new mafMatrix(m_Matrix->rows, m_Matrix->cols);
    cvCopy(m_Matrix, m->m_Matrix);
    return m;
}

void mafMatrix::setIdentity() {
    cvSetIdentity(m_Matrix);
}

mafMatrix &mafMatrix::operator *(const mafMatrix &mat) {
    mafMatrix m(m_Matrix->rows, mat.m_Matrix->cols);
    cvMatMul(m_Matrix, mat.m_Matrix, m.m_Matrix);
    
    if(m_Matrix->cols != m.m_Matrix->cols) {
        cvReleaseMat(&m_Matrix);
        m_Matrix = cvCreateMat(m.m_Matrix->rows, m.m_Matrix->cols, CV_64FC1);
    }
    
    cvCopy(m.m_Matrix, m_Matrix);
    return *this;
}

void mafMatrix::setElement(int row, int col, double value) {
    cvmSet(m_Matrix, row, col, value);
}

double mafMatrix::element(int row, int col) {
    return cvmGet(m_Matrix, row, col);
}
