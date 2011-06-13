/*
 *  mafMatrix.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMATRIX_H
#define MAFMATRIX_H

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
    mafMatrix(int rows = 4, int cols = 4);
    
    // clone matrix to a new instance
    mafMatrix *clone() const;
        
    /// set the current matrix as identity matrix
    void setIdentity();
    
    /// multiplication matrix operator 
    mafMatrix operator *(const mafMatrix &mat) const;
    
    /// assignment matrix operator 
    mafMatrix &operator =(const mafMatrix &mat);

        
    /// set the value for element at given row and column
    void setElement(int row, int col, double value);
    
    /// return the element at the given row and column.
    double element(int row, int col);
        
    /// Object destructor.
    /* virtual */ ~mafMatrix();
    
protected:

    CvMat *m_Matrix;///< represents the basic matrix

private:
    

};

} // namespace mafPluginVTK


#endif // MAFMATRIX_H