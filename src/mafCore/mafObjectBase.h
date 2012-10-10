/*
 *  mafObjectBase.h
 *  mafCore
 *  
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *  
 *  See License at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFOBJECTBASE_H
#define MAFOBJECTBASE_H

// Includes list
#include "mafCore_global.h"
#include "mafDelegate.h"


namespace mafCore {

// Class forwarding list
class mafVisitor;


/**
 Class name: mafObjectBase
 This class defines the MAF3 base object and visited by the mafVisitor class.
 */
class MAFCORESHARED_EXPORT mafObjectBase : public mafDelegate {
    Q_OBJECT
    Q_PROPERTY(QString objectHash READ objectHash WRITE setObjectHash)
    Q_PROPERTY(QObject *widget READ widget WRITE setWidget STORED false)
    Q_PROPERTY(mafCore::mafDelegatePointer delegateObject READ delegateObject WRITE setDelegateObject STORED false)
    Q_PROPERTY(QString uiFile READ uiFilename)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafDelegate);
    
Q_SIGNALS:
    /// Signal emitted to alert all receivers that the object has been modified.
    void modifiedObject();

    /// Signal emitted to update GUI.
    void updateGuiSignal(QObject *selfUI);

public Q_SLOTS:
    /// update ui widgets with properties, using USER flag in Q_PROPERTY.
    void updateUI(QObject *selfUI);

public:
    /// Object constructor.
    mafObjectBase(const QString code_location = "");

    /// Initialize the object.
    virtual bool initialize();

    /// Return the object's ID.
    mafId objectId() const;

    /// Used in MAF3 design by contract to check object's validity.
    /** isObjectValid methods play an important role in checking the consistency
    of objects in the debug. isObjectValid is defined as a pure virtual function
    in Object class, thus it needs to be overridden in all inheriting classes.
    The inheriting class should perform defensive checks to make
    sure that it is in a consistent state/
    Also note that this method is only available in the debug build.*/
    /*virtual*/ bool isObjectValid() const;

    /// Check if the object is equal to that passed as argument. NB. The objectHash is not considered as argument of comparision.
    virtual bool isEqual(const mafObjectBase *obj) const;

    /// Redefined == operator which call isEqual method to check the equality of the current object with that one passed as argument.
    bool operator==(const mafObjectBase &obj) const;

    /// Return the hash associated to the object.
    const QString objectHash() const;
    
    /// Set the modified state of the mafObject.
    void setModified(bool m = true);
    
    /// Return the modified state of the mafObject.
    bool modified() const;

    /// Allow to assign a custom widget to the class instead of UI filename to be loaded.
    /** This method initialize the m_Widget variable which may be created from code or loaded from xml.
    Assigning it will cause the m_UIFilename to be overwritten with the empty string.*/
    void setWidget(QObject *w);

    /// Return the pointer to the root widget associated with the object.
    QObject *widget() const;

    /// Return the filename associated to the object's UI.
    const QString uiFilename() const;

    /// Return the delegate class pointer.
    mafDelegatePointer delegateObject() const;
    
    /// Assign the delegate class pointer to the object.
    void setDelegateObject(mafDelegatePointer d);

    /// Allows to accept a mafVisitor which will visit the object and will be executed the mafVisitor algorithm.
    virtual void acceptVisitor(mafVisitor *v);

    /// Allows the generic connection, using the Qt notation on_ObjectName1_signal with on_ObjectName2_slot.
    void connectObjectSlotsByName(QObject *signal_object);

    /// dump the description of the object (information, attributes, variables...)
    virtual void description() const;

protected:
    /// set the hash code for the current object.
    /** This method is used from the undo mechanism like memento pattern
    and by the serialization mechanism to restore the previous saved object's hash.*/
    void setObjectHash(const QString obj_hash);
   
    QString m_UIFilename; ///< Filename that define the object's UI written into a .ui XML file.
    QObject *m_Widget; ///<  widget of the UI class allocated and given to the mafObjectBase. It can be obtained from ui file or from a direct instantiation.
    /// Object destructor.
    virtual ~mafObjectBase();

private:
    mafId m_ObjectId; ///< Unique ID which identifies the object.
    QUuid m_ObjectHash; ///< Hash value for the current object.
    bool m_Modified; ///< Contains the modified state of the VME.
    mafDelegatePointer m_Delegate; ///< Delegate class pointer.
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
    
inline bool mafObjectBase::modified() const {
    return m_Modified;
}

inline void mafObjectBase::setObjectHash(const QString obj_hash) {
    m_ObjectHash = QUuid(obj_hash);
}

inline const QString mafObjectBase::uiFilename() const {
    return m_UIFilename;
}

inline void mafObjectBase::setWidget(QObject *w) {
    m_Widget = w;
}

inline QObject *mafObjectBase::widget() const {
    return m_Widget;
}
    
inline mafDelegatePointer mafObjectBase::delegateObject() const {
    return m_Delegate;
}

inline void mafObjectBase::setDelegateObject(mafDelegatePointer d) {
    m_Delegate = d;
}

inline bool mafObjectBase::operator ==(const mafObjectBase& obj) const {
    return this->isEqual(&obj);
}

} // namespace mafCore

#endif  // MAFOBJECTBASE_H
