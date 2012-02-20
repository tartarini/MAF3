/*
 *  mafPoint.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *  
 *  See License at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFPOINT_H
#define MAFPOINT_H

// Includes list
#include "mafReferenceCounted.h"

namespace mafCore {

/**
 Class name: mafPoint
 This class defines the 3D point.
 */
class MAFCORESHARED_EXPORT mafPoint : public mafReferenceCounted {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafReferenceCounted);

public:
    /// Object constructor
    mafPoint(double x_pos, double y_pos, double z_pos);
    
    /// Object constructor
	mafPoint(double pos[3], const QString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafPoint();

    /// Object constructor
    mafPoint(const QString code_location = "");	
    
    /// Redefined = operator which allows to copy values from another mafPoint class.
    mafPoint &operator=(const mafPoint &obj);
    
    /// check if two bounds are equal.
    bool operator==(const mafPoint &obj) const;

    /// check if two bounds are different.
    bool operator!=(const mafPoint &obj) const;
    
    /// Return the X coordinate.
    double x() const;
    
    /// Return the Y coordinate.
    double y() const;

    /// Return the Z coordinate.
    double z() const;

    /// Assign the X coordinate.
    void setX(double x_coord);
    
    /// Assign the Y coordinate.
    void setY(double y_coord);

    /// Assign the Z coordinate.
    void setZ(double z_coord);

    /// Reset the point coordinate to 0,0,0.
    void zero();

    /// fill the array with current point coordinates.
    void pos(double p[3]);

private:
	double m_X; ///< X coordinate
    double m_Y; ///< Y coordinate
    double m_Z; ///< Z coordinate
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline double mafPoint::x() const {
    return m_X;
}

inline double mafPoint::y() const {
    return m_Y;
}

inline double mafPoint::z() const {
    return m_Z;
}

inline void mafPoint::setX(double x_coord) {
    m_X = x_coord;
}

inline void mafPoint::setY(double y_coord) {
    m_Y = y_coord;
}

inline void mafPoint::setZ(double z_coord) {
    m_Z = z_coord;
}

inline void mafPoint::zero() {
    m_X = m_Y = m_Z = 0.;
}

typedef mafPoint * mafPointPointer;

} // namespace mafCore

Q_DECLARE_METATYPE(mafCore::mafPointPointer);

#endif  // MAFPOINT_H
