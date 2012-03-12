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
    // Create the instance of OpenCV 4x4 double value matrix.
    m_Matrix = cvCreateMat(4, 4, CV_64FC1);
}

mafMatrix::mafMatrix(int rows, int cols) {
    // Create the instance of generic dimensioned OpenCV double value matrix.
    m_Matrix = cvCreateMat(rows, cols, CV_64FC1);
}

mafMatrix::mafMatrix(const mafMatrix &m) {
    // Copy constructor for 4x4 double value matrix.
    m_Matrix = cvCreateMat(m.m_Matrix->rows, m.m_Matrix->cols, CV_64FC1);
    *this = m;
}

mafMatrix::~mafMatrix() {
    cvReleaseMat(&m_Matrix);
}

mafMatrix & mafMatrix::operator =(const mafMatrix &mat) {
    // Assignment operator overload.
    cvCopy(mat.m_Matrix, m_Matrix);
    return *this;
}

double *mafMatrix::rawData() const {
    // Return the pointer of raw data contained into the matrix
    // as row-wise values.
    return m_Matrix->data.db;
}

mafMatrix *mafMatrix::clone() const {
    // Return a cloned matrix. The memory has to be released by who required the clone.
    mafMatrix *m = new mafMatrix(m_Matrix->rows, m_Matrix->cols);
    cvCopy(m_Matrix, m->m_Matrix);
    return m;
}

bool mafMatrix::isEqual(const mafMatrix &mat) {
    // First check is on number of rows and cols
    CvMat *check = mat.m_Matrix;
    if(m_Matrix->rows != check->rows || m_Matrix->cols != check->cols) {
        return false;
    }
    
    int r = m_Matrix->rows;
    int c = m_Matrix->cols;
    int dim = r * c;
    int i=0;

    // Then cycle on all values and check their values using mafEqual macro.
    double *me = rawData();
    double *other = mat.rawData();
    for (; i < dim; ++i) {
        if (!mafEquals(me[i], other[i])) {
            qDebug() << mafTr("Not equal at index: ") << i;
            qDebug() << mafTr("first:  ") << me[i];
            qDebug() << mafTr("second: ") << other[i];
            return false;
        }
    }

    return true;
}

void mafMatrix::description() const {
    // Print the information contained into the class.
    int r = m_Matrix->rows;
    int c = m_Matrix->cols;
    qDebug() << mafTr("Number of Rows: ") << r; 
    qDebug() << mafTr("Number of Cols: ") << c;
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
    // Assign the identity matrix to the internal matrix instance.
    // All previous values are overwritten.
    cvSetIdentity(m_Matrix);
}

mafMatrix mafMatrix::operator *(const mafMatrix &mat) {
    // Overload of the '*' operator to perform the matrix multiplication.
    mafMatrix result(m_Matrix->rows, mat.m_Matrix->cols);
    result.setIdentity();
    cvMatMul(m_Matrix, mat.m_Matrix, result.m_Matrix);
    
    return result;
}

void mafMatrix::setElement(int row, int col, double value) {
    // Assign the value 'value' at the element (row, col).
    cvmSet(m_Matrix, row, col, value);
}

double mafMatrix::element(int row, int col) {
    // Return the element located at (row, col)
    return cvmGet(m_Matrix, row, col);
}

void mafMatrix::setColumn(mafMatrix &col, int col_index) {
    if (m_Matrix->rows == col.m_Matrix->rows && col_index < m_Matrix->cols && col_index >= 0) {
        for (int r = 0; r < m_Matrix->rows; ++r) {
            setElement(r, col_index, col.element(r, 0));
        }
    }
}

void mafMatrix::setRow(mafMatrix &row, int row_index) {
    if (m_Matrix->cols == row.m_Matrix->cols && row_index < m_Matrix->rows && row_index >= 0) {
        for (int c = 0; c < m_Matrix->cols; ++c) {
            setElement(row_index, c, row.element(0, c));
        }
    }
}
