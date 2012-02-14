/*
 *  mafBounds.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *  
 *  See License at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFBOUNDS_H
#define MAFBOUNDS_H

// Includes list
#include "mafReferenceCounted.h"

namespace mafCore {

/**
 Class name: mafBounds
 This class defines the 3D bounds used by VMEs.
 */
class MAFCORESHARED_EXPORT mafBounds : public mafReferenceCounted {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafReferenceCounted);

public:
    /// Object destructor.
    /* virtual */ ~mafBounds();

    /// Object constructor
    mafBounds(const QString code_location = "");
	
    /// Object constructor
//	mafBounds(const mafBounds &p);

    /// Redefined = operator which allows to copy values from another mafBounds class.
    mafBounds &operator=(const mafBounds &obj);

    /// Object constructor
	mafBounds(double pos[6], const QString code_location = "");
    
    /// Check if the bounds are valid.
	bool isValid();
	
	/// Calculate the union of the given bounds with this one.
	void unite(const mafBounds &b);
	
	/// Calculate the intersection with the given bounds.
	void intersect(const mafBounds &b);

    /// Return the minimum X coordinate
    double xMin() const;

    /// Return the maximum X coordinate
    double xMax() const;

    /// Return the minimum Y coordinate
    double yMin() const;

    /// Return the maximum Y coordinate
    double yMax() const;

    /// Return the minimum Z coordinate
    double zMin() const;

    /// Return the maximum Z coordinate
    double zMax() const;

private:
	double m_XMin; ///< Minimum X value
    double m_YMin; ///< Minimum Y value
    double m_ZMin; ///< Minimum Z value
    double m_XMax; ///< Maximum X value
    double m_YMax; ///< Maximum Y value
    double m_ZMax; ///< Maximum Z value
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline double mafBounds::xMin() const {
    return m_XMin;
}

inline double mafBounds::xMax() const {
    return m_XMax;
}

inline double mafBounds::yMin() const {
    return m_YMin;
}

inline double mafBounds::yMax() const {
    return m_YMax;
}

inline double mafBounds::zMin() const {
    return m_ZMin;
}

inline double mafBounds::zMax() const {
    return m_ZMax;
}

typedef mafBounds * mafBoundsPointer;

} // namespace mafCore

Q_DECLARE_METATYPE(mafCore::mafBoundsPointer);

#endif  // MAFBOUNDS_H
