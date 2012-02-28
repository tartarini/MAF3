/*
 *  mafPoint.h
 *  mafResources
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
#include "mafResourcesDefinitions.h"
#include <mafReferenceCounted.h>

namespace mafResources {

/**
 Class name: mafPoint
 This class defines the 3D point.
 */
    class MAFRESOURCESSHARED_EXPORT mafPoint : public mafCore::mafReferenceCounted {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafReferenceCounted);

public:
    /// Object constructor
    mafPoint(double x_pos, double y_pos, double z_pos);
    
    /// Object constructor
	mafPoint(double pos[3], const QString code_location = "");

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

    /// set in a one shot all the coordinates.
    void setPosition(double pos[3]);
    
    /// Reset the point coordinate to 0,0,0.
    void zero();

    /// fill the array with current point coordinates.
    void pos(double p[3]);

protected:
    /// Object destructor.
    /* virtual */ ~mafPoint();
    
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

    
inline void mafPoint::setPosition(double pos[3]) {
    m_X = pos[0];
    m_Y = pos[1];
    m_Z = pos[2];
}    

inline void mafPoint::zero() {
    m_X = m_Y = m_Z = 0.;
}

typedef mafPoint * mafPointPointer;

} // namespace mafResources

Q_DECLARE_METATYPE(mafResources::mafPointPointer);

#endif  // MAFPOINT_H
