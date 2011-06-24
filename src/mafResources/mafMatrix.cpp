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
#include <QDebug>

using namespace cv;
using namespace mafResources;
using namespace mafCore;

mafMatrix::mafMatrix() {
    m_Matrix = cvCreateMat(4, 4, CV_64FC1);
}

mafMatrix::mafMatrix(int rows, int cols) {
    m_Matrix = cvCreateMat(rows, cols, CV_64FC1);
}

mafMatrix::mafMatrix(const mafMatrix &m) {
    m_Matrix = cvCreateMat(4, 4, CV_64FC1);
    *this = m;
}

mafMatrix::~mafMatrix() {
    cvReleaseMat(&m_Matrix);
}

mafMatrix & mafMatrix::operator =(const mafMatrix &mat) {
    cvCopy(mat.m_Matrix, m_Matrix);
    return *this;
}

double *mafMatrix::rawData() const {
    return m_Matrix->data.db;
}

mafMatrix *mafMatrix::clone() const {
    mafMatrix *m= new mafMatrix(m_Matrix->rows, m_Matrix->cols);
    cvCopy(m_Matrix, m->m_Matrix);
    return m;
}

bool mafMatrix::isEqual(const mafMatrix &mat) {
    CvMat *check = mat.m_Matrix;
    if(m_Matrix->rows != check->rows || m_Matrix->cols != check->cols) {
        return false;
    }
    
    int r = m_Matrix->rows;
    int c = m_Matrix->cols;
    int dim = r * c;
    int i=0;

    double *me = rawData();
    double *other = mat.rawData();
    for (; i < dim; ++i) {
        if (!mafEquals(me[i], other[i])) {
            return false;
        }
    }

    return true;
}

void mafMatrix::description() const {
    int r = m_Matrix->rows;
    int c = m_Matrix->cols;
    qDebug() << "Number of Rows: " << r; 
    qDebug() << "Number of Cols: " << c;
    int i=0;
    int j=0;
    double value;
    for (; i<r ; ++i){
        QString rowValues;
        for(j = 0; j<c ; ++j) {
            value = cvmGet(m_Matrix, i, j);
            rowValues.append(QString::number(value)).append(" ");
        }
        qDebug() << rowValues;
    }

}

void mafMatrix::setIdentity() {
    cvSetIdentity(m_Matrix);
}

mafMatrix mafMatrix::operator *(const mafMatrix &mat) {
    mafMatrix result(m_Matrix->rows, mat.m_Matrix->cols);
    cvMatMul(m_Matrix, mat.m_Matrix, result.m_Matrix);
    
    return result;
}

void mafMatrix::setElement(int row, int col, double value) {
    cvmSet(m_Matrix, row, col, value);
}

double mafMatrix::element(int row, int col) {
    return cvmGet(m_Matrix, row, col);
}
