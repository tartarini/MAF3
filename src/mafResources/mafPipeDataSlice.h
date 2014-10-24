/*
 *  mafPipeDataSlice.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 18/1/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEDATASLICE_H
#define MAFPIPEDATASLICE_H

// Includes list
#include "mafResourcesDefinitions.h"
#include "mafPipeData.h"

// Foundation Class forwarding list


namespace mafResources {

typedef enum {
    mafPlaneNormalX = 0,
    mafPlaneNormalY,
    mafPlaneNormalZ,
    mafPlaneNormalArbitrary
} mafPlaneNormal;


// Class forwarding list

/**
 Class name: mafPipeDataSlice
 This is the base class for slicing strategy pattern.
 */
class MAFRESOURCESSHARED_EXPORT mafPipeDataSlice : public mafPipeData {
    Q_OBJECT

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

public:
    /// Object constructor.
    mafPipeDataSlice(const QString code_location = "");

    /// Set the origin of the slice
    void setSliceOrigin(double *origin);

    /// Set the origin of the slice
    void setSliceOrigin(double origX, double origY, double origZ);

    /// Return the slice origin.
    double *sliceOrigin();

    /// Set the normal of the slice
    void setNormal(double *normal);

    /// Return the slice normal.
    double *normal();

    /// Return the plane normal axes.
    mafPlaneNormal planeNormalAxes() const;

    /// Assign the plane normal axes.
    void setPlaneNormalAxes(mafPlaneNormal axes);

    /// Allows to move the slice along its normal of 'distance' amount.
    void pushSlice(double distance);

    /// Assign the transform matrix to use for the cut.
    void setTransformMatrix(mafMatrix4x4 &m);

    /// Return the transform matrix used for the cut.
    const mafMatrix4x4 *transformMatrix() const;

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeDataSlice();

    /// Return the X vector that defines the slicing plane.
    const double *xVector() const;

    /// Return the Y vector that defines the slicing plane.
    const double *yVector() const;

    mafMatrix4x4 m_TransformMatrix; ///< Transformation Matrix to use for the cut.

private:
    double m_SliceOrigin[3]; ///< Origin of the slice.
    double m_Normal[3]; ///< Normal vector of the slice.
    double m_XVector[3];
    double m_YVector[3];

    mafPlaneNormal m_PlaneNormalAxes; ///< Constant indicating the axes on which lies the plane normal.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafPipeDataSlice::setSliceOrigin(double *origin) {
    REQUIRE(origin);

    m_SliceOrigin[0] = origin[0];
    m_SliceOrigin[1] = origin[1];
    m_SliceOrigin[2] = origin[2];
}

inline void mafPipeDataSlice::setSliceOrigin(double origX, double origY, double origZ) {
    m_SliceOrigin[0] = origX;
    m_SliceOrigin[1] = origY;
    m_SliceOrigin[2] = origZ;
}

inline double *mafPipeDataSlice::sliceOrigin() {
    return m_SliceOrigin;
}

inline double *mafPipeDataSlice::normal() {
    return m_Normal;
}

inline mafPlaneNormal mafPipeDataSlice::planeNormalAxes() const {
    return m_PlaneNormalAxes;
}

inline const double *mafPipeDataSlice::xVector() const {
    return &m_XVector[0];
}

inline const double *mafPipeDataSlice::yVector() const {
    return &m_YVector[0];
}

inline const mafMatrix4x4 *mafPipeDataSlice::transformMatrix() const {
    return &m_TransformMatrix;
}

} // namespace mafResources

#endif // MAFPIPEDATASLICE_H
