/*
 *  mafObjectBase.h
 *  mafCore
 *  
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFOBJECTBASE_H
#define MAFOBJECTBASE_H

// Includes list
#include "mafCore_global.h"

namespace mafCore {

// Class forwarding list
class mafVisitor;

/**
 Class name: mafObjectBase
 This class defines the MAF3 base object and visited by the mafVisitor class.
 */
class MAFCORESHARED_EXPORT mafObjectBase : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString objectHash READ objectHash WRITE setObjectHash)
    /// typedef macro.
    mafSuperclassMacro(QObject);

public:
    /// Object constructor.
    mafObjectBase(const mafString code_location = "");

    /// Object destructor.
    virtual ~mafObjectBase();

    /// Return the object's ID.
    mafId objectId() const;

    /// Used in MAF3 design by contract to check object's validity.
    /** isObjectValid methods play an important role in checking the consistency
    of objects in the debug. isObjectValid is defined as a pure virtual function
    in Object class, thus it needs to be overridden in all inheriting classes.
    The inheriting class should perform defensive checks to make
    sure that it is in a consistent state/
    Also note that this method is only available in the debug build.*/
    virtual bool isObjectValid() const;

    /// Check if the object is equal to that passed as argument. NB. The objectHash is not considered as argument of comparision.
    virtual bool isEqual(const mafObjectBase *obj) const;

    /// Redefined == operator which call isEqual method to check the equality of the current object with that one passed as argument.
    bool operator==(const mafObjectBase &obj) const;

    /// Return the hash associated to the object.
    const mafString objectHash() const;

    /// Create the hash code given the string as argument. The hash is calculated using the SHA1 encryption algorithm.
//    void createHashCode(mafString &token);

    /// Allows to accept a mafVisitor which will visit the object and will be executed the mafVisitor algorithm.
    virtual void acceptVisitor(mafVisitor *v);

protected:
    /// set the hash code for the current object.
    /** This method is used from the undo mechanism like memento pattern
    and by the serialization mechanism to restore the previous saved object's hash.*/
    void setObjectHash(const mafString obj_hash);
    
signals:
    /// Signal emitted to alert all receivers that the object has been modified.
    void modifiedObject();

private:
    mafId m_ObjectId; ///< Unique ID which identifies the object.
    //mafByteArray m_ObjectHash; ///< Hash value for the current object.
    mafUuid m_ObjectHash; ///< Hash value for the current object.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafId mafObjectBase::objectId() const {
    return m_ObjectId;
}

inline const mafString mafObjectBase::objectHash() const {
    return m_ObjectHash.toString();
}

inline void mafObjectBase::setObjectHash(const mafString obj_hash) {
    m_ObjectHash = mafUuid(obj_hash);
}

inline bool mafObjectBase::operator ==(const mafObjectBase& obj) const {
    return this->isEqual(&obj);
}

} // mafCore

#endif  // MAFOBJECTBASE_H
