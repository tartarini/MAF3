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
    /// Object destructor.
    /* virtual */ ~mafPoint();

    /// Object constructor
    mafPoint(const QString code_location = "");
	
    /// Object constructor
//	mafPoint(const mafPoint &p);

    /// Object constructor
    mafPoint(double x_pos, double y_pos, double z_pos);
    
    /// Object constructor
	mafPoint(double pos[3], const QString code_location = "");

    /// Redefined = operator which allows to copy values from another mafPoint class.
    mafPoint &operator=(const mafPoint &obj);

    /// Return the X coordinate.
    double x() const;
    
    /// Return the Y coordinate.
    double y() const;

    /// Return the Z coordinate.
    double z() const;

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

typedef mafPoint * mafPointPointer;

} // namespace mafCore

Q_DECLARE_METATYPE(mafCore::mafPointPointer);

#endif  // MAFPOINT_H
