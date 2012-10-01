/*
 *  mafMatrix.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMATRIX_H
#define MAFMATRIX_H

#include <QMetaType>
#include <mafCoreDefinitions.h>

// OpenCV includes
#include <cv.h>

#include "mafResources_global.h"

namespace mafResources {
    
/**
 Class Name: mafMatrix
 mafMatrix represent the extended matrix inheriting from opencv cv::Mat.
 Added  setToIdentity and modify element.
 */
class MAFRESOURCESSHARED_EXPORT mafMatrix {
    
public:
    /// Object constructor.
    mafMatrix();
    
    /// Object constructor for generic row,column numbers matrix.
    mafMatrix(int rows, int cols);
    
    /// Object destructor.
    /* virtual */ ~mafMatrix();
    
    /// Copy Constructor
    mafMatrix(const mafMatrix &m);

    // clone matrix to a new instance.
    mafMatrix *clone() const;
        
    /// set the current matrix as identity matrix.
    void setIdentity();
    
    /// multiplication matrix operator.
    mafMatrix operator *(const mafMatrix &mat);
    
    /// assignment matrix operator 
    mafMatrix &operator =(const mafMatrix &mat);

    /// set the value for element at given row and column.
    void setElement(int row, int col, double value);
    
    /// return the element at the given row and column.
    double element(int row, int col);

    /// Set the given column into the matrix at the given index.
    void setColumn(mafMatrix &col, int col_index);

    /// Return the column at the given index.
    mafMatrix extractColumn(unsigned int col);
    
    /// Set the given row into the matrix at the given index.
    void setRow(mafMatrix &row, int row_index);

    /// Return the row at the given index.
    mafMatrix extractRow(unsigned int row);

    /// check if the matrix parameter is equal.
    bool isEqual(const mafMatrix &mat);
    
    /// dump the description of the object (information, attributes, variables...)
    virtual void description() const;
    
    /// Return the raw data (row-wise) of the matrix.
    double *rawData() const;
    
private:
    CvMat *m_Matrix;///< represents the basic matrix.    
};
    
/// Define the pointer to the mafMatrix needed to be registered with Q_DECLARE_METATYPE macro.
typedef mafMatrix * mafMatrixPointer;

} // namespace mafResources

Q_DECLARE_METATYPE(mafResources::mafMatrixPointer);
Q_DECLARE_METATYPE(mafResources::mafMatrix);

#endif // MAFMATRIX_H