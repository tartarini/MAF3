/*
 *  mafObjectBase.h
 *  mafCore
 *  
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 B3C. All rights reserved.
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
class MAFCORESHARED_EXPORT mafObjectBase : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString objectHash READ objectHash WRITE setObjectHash)
    Q_PROPERTY(QObject *uiRootWidget READ uiRootWidget WRITE setUiRootWidget STORED false)
    Q_PROPERTY(mafCore::mafDelegatePointer delegate READ delegate WRITE setDelegate STORED false)
    Q_PROPERTY(QString uiFile READ uiFilename)
    /// typedef macro.
    mafSuperclassMacro(QObject);
    
Q_SIGNALS:
    /// Signal emitted to alert all receivers that the object has been modified.
    void modifiedObject();
    
    /// Allows to increment the reference count.
    void incrementReference();
    
    /// Allows to decrement the reference count.
    void decreaseReference();

    /// Signal emitted to update GUI.
    void updateGuiSignal(QObject *selfUI);

public Q_SLOTS:
    /// update ui widgets with properties, using USER flag in Q_PROPERTY.
    void updateUI(QObject *selfUI = NULL);

private Q_SLOTS:
    /// increment of 1 unit the reference count.
    void ref();

    /// delete the object.
    void deleteObject();

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
    virtual bool isObjectValid() const;

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

    /// Return the filename associated to the object's UI.
    const QString uiFilename() const;

    /// Return the pointer to the root widget associated with the object.
    QObject *uiRootWidget() const;

    /// Allows to assign the root widget to associate to the object.
    void setUiRootWidget(QObject *w);
    
    /// Return the delegate class pointer.
    mafDelegatePointer delegate() const;
    
    /// Assign the delegate class pointer to the object.
    void setDelegate(mafDelegatePointer d);

    /// Allows to accept a mafVisitor which will visit the object and will be executed the mafVisitor algorithm.
    virtual void acceptVisitor(mafVisitor *v);

    /// Allows the generic connection, using the Qt notation on_ObjectName1_signal with on_ObjectName2_slot.
    void connectObjectSlotsByName(QObject *signal_object);

    /// Allows to Q_EMIT the incrementReference in a thread safe way.
    void retain();
    
    /// Allows to decrease the reference count of the object.
    void release();
    
    /// return the reference count.
    int referenceCount() const;
    
    /// dump the description of the object (information, attributes, variables...)
    virtual void description() const;

protected:
    /// set the hash code for the current object.
    /** This method is used from the undo mechanism like memento pattern
    and by the serialization mechanism to restore the previous saved object's hash.*/
    void setObjectHash(const QString obj_hash);
   
    QString m_UIFilename; ///< Filename that define the object's UI written into a .ui XML file. m_UIFilename and m_UIRootWidget are mutually exclusive.
    QObject *m_UIRootWidget; ///< Root widget of the UI class allocated and given to the mafObjectBase instead of assigning the m_UIFilename of the ".ui" file to load. m_UIFilename and m_UIRootWidget are mutually exclusive.

    /// Object destructor.
    virtual ~mafObjectBase();

private:
    mafId m_ObjectId; ///< Unique ID which identifies the object.
    //QByteArray m_ObjectHash; ///< Hash value for the current object.
    QUuid m_ObjectHash; ///< Hash value for the current object.
    bool m_Modified; ///< Contains the modified state of the VME.
    QObject *m_SelfUI; ///< Ui object;
    mafDelegatePointer m_Delegate; ///< Delegate class pointer.

    volatile int m_ReferenceCount; ///< Index containing the reference count.
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

inline void mafObjectBase::setUiRootWidget(QObject *w) {
    m_UIRootWidget = w;
    m_UIFilename = "";
}

inline QObject *mafObjectBase::uiRootWidget() const {
    return m_UIRootWidget;
}
    
inline mafDelegatePointer mafObjectBase::delegate() const {
    return m_Delegate;
}

inline void mafObjectBase::setDelegate(mafDelegatePointer d) {
    m_Delegate = d;
}

inline bool mafObjectBase::operator ==(const mafObjectBase& obj) const {
    return this->isEqual(&obj);
}

inline int mafObjectBase::referenceCount() const{
    return m_ReferenceCount;
}

} // mafCore

#endif  // MAFOBJECTBASE_H
