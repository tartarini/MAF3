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
    Q_PROPERTY(QString uiFile READ uiFilename)
    /// typedef macro.
    mafSuperclassMacro(QObject);

public:
    /// Object constructor.
    mafObjectBase(const QString code_location = "");

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
    const QString objectHash() const;

    /// Return the filename associated to the object's UI.
    const QString uiFilename() const;

    /// Allows to accept a mafVisitor which will visit the object and will be executed the mafVisitor algorithm.
    virtual void acceptVisitor(mafVisitor *v);

    /// Allows the generic connection, using the Qt notation on_ObjectName1_signal with on_ObjectName2_slot.
    void connectObjectSlotsByName(QObject *signal_object);

    /// initialize ui widgets with properties, using USER flag in Q_PROPERTY.
    void initializeUI(QObject *selfUI);

public slots:
    /// increment of 1 unit the reference count.
    void ref();

    /// delete the object.
    void deleteObject();

    /// return the reference count.
    int referenceCount();

protected:
    /// set the hash code for the current object.
    /** This method is used from the undo mechanism like memento pattern
    and by the serialization mechanism to restore the previous saved object's hash.*/
    void setObjectHash(const QString obj_hash);
    
    QString m_UIFilename; ///< Filename that define the object's UI written into a XML file.

    /// Object destructor.
    virtual ~mafObjectBase();

signals:
    /// Signal emitted to alert all receivers that the object has been modified.
    void modifiedObject();

private:
    mafId m_ObjectId; ///< Unique ID which identifies the object.
    //QByteArray m_ObjectHash; ///< Hash value for the current object.
    QUuid m_ObjectHash; ///< Hash value for the current object.

    int m_ReferenceCount; ///< Index containing the reference count.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafId mafObjectBase::objectId() const {
    return m_ObjectId;
}

inline const QString mafObjectBase::objectHash() const {
    return m_ObjectHash.toString();
}

inline void mafObjectBase::setObjectHash(const QString obj_hash) {
    m_ObjectHash = QUuid(obj_hash);
}

inline const QString mafObjectBase::uiFilename() const {
    return m_UIFilename;
}

inline bool mafObjectBase::operator ==(const mafObjectBase& obj) const {
    return this->isEqual(&obj);
}

inline void mafObjectBase::ref() {
    ++m_ReferenceCount;
}

inline int mafObjectBase::referenceCount() {
    return m_ReferenceCount;
}

} // mafCore

#endif  // MAFOBJECTBASE_H
