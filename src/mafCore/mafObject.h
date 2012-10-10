/*
 *  mafObject.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFOBJECT_H
#define MAFOBJECT_H

// Includes list
#include "mafObjectBase.h"

Q_DECLARE_FLAGS(mafObjectLockStatus, mafCore::mafObjectLock)

namespace mafCore {

// Class forwarding list
class mafMemento;


/**
 Class name: mafObject
 This class defines the basic API for objects that wants to be serialized .
 */
class MAFCORESHARED_EXPORT mafObject : public mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(QString iconType READ iconType WRITE setIconType)
    Q_PROPERTY(int lockStatus READ lockStatus)
    Q_PROPERTY(int progressStatus READ progressStatus WRITE setProgressStatus)

    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafObject(const QString code_location = "");

    /// Used in MAF3 design by contract to check object's validity.
    /** isObjectValid methods play an important role in checking the consistency
    of objects in the debug. isObjectValid is defined as a pure virtual function
    in Object class, thus it needs to be overridden in all inheriting classes.
    The inheriting class should perform defensive checks to make
    sure that it is in a consistent state/
    Also note that this method is only available in the debug build.*/
    /*virtual*/ bool isObjectValid() const;

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Allow to set the icon type representing the icon file associated with the object.
    void setIconType(QString icon_type);

    /// Return the icon type associated with the object.
    QString iconType() const;

    /// Return the instance of the object's status. The caller has to delete the allocated memento memory he asked for.
    virtual mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization purposes.*/
    virtual void setMemento(mafMemento *memento, bool deep_memento = false);
    
    /// Add string tag to the tag list, that will categorize the object.
    /** This list of tags wants to define a series of strings that describe the object and
    categorize it in the cloud of MAF3 objects (like photo tags on Flickr).
    This method return true if the tag is added correctly, otherwise false.
    It return false also if you are trying to add and already added tag.*/
    bool addTag(QString tag);

    /// Remove from the tag list the given string tag.
    bool removeTag(QString tag);

    /// Find all the tags that match the given string patter and put the result into the 'filtered_list argument.
    void filterTags(QString tag_pattern, QVariantList *filtered_list);

    /// Return the list of all tag inserted.
    const QVariantList *tagList() const;

    /// Return an instance of the dictionary containing keys needed for script information.
    /** This method create the dictionary needed for the script list and contains the following keys:
    - script: indicate the script as file path or as script string.
    - type:   indicate the type of script (mafScriptTypeFilePath or mafScriptTypeStringScript)
    - interpreter: indicate the interpreter needed to execute the script (python, ecma, ...)
    The standard work-flow usage is to ask the object generate a new script information 
    dictionary (by calling this method), customize its information and add it to the list 
    by calling the addScript method.
    */
    QVariantHash *scriptDictionary(QString interpreter = "python");

    /// Add a script to the scriptList.
    bool addScript(QVariantHash *script);

    /// Remove from the script list the given script.
    bool removeScript(QVariantHash *script);

    /// Remove from the script list the script at given index.
    bool removeScript(int scriptIndex);

    /// Return the list of all scripts inserted.
    const QVariantList *scriptList() const;

    /// Return the dictionary associated to the object.
    QVariantMap *dictionary() const;

    /// Return the lock status of the object.
    int lockStatus() const;

    /// Set value of progress status.
    void setProgressStatus(int status);

    /// Return status of progress to be used in a progress bar.
    int progressStatus() const;

    /// Method used to initialize the object's script list.
    void setScriptList(const QVariantList &list);

public Q_SLOTS:
    /// Method used to initialize the object's tag list.
    void setTagList(const QVariantList &list);

protected Q_SLOTS:
    /// fill the dictionary with object hash.
    virtual void fillDictionary();

protected:
    /// Object destructor.
    /* virtual */ ~mafObject();
    bool m_Selected; ///< Flag used to store the selection flag of the owned VME.
    QString m_IconType; ///< Filename associated to the icon file to use with the object.
    mafObjectLockStatus m_LockStatus; ///< Contains the status of object lock.
    int m_ProgressStatus; ///< Contains the progress status.

private:
    QVariantList *m_TagList; ///< Tag list that categorize the object.
    QVariantList *m_ScriptList; ///< List containing scripts associated to the object.
    QVariantMap* m_Dictionary; ///< Dictionary associated to the object.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QString mafObject::iconType() const {
    return m_IconType;
}

inline const QVariantList *mafObject::tagList() const {
    return m_TagList;
}

inline const QVariantList *mafObject::scriptList() const {
    return m_ScriptList;
}

inline QVariantMap *mafObject::dictionary() const {
    return m_Dictionary;
}

inline int mafObject::lockStatus() const {
    return m_LockStatus;
}

inline int mafObject::progressStatus() const {
    return m_ProgressStatus;
}

inline QVariantHash *mafObject::scriptDictionary(QString interpreter) {
    QVariantHash *dic = new QVariantHash();
    dic->insert(mafScriptKey, QVariant(""));
    dic->insert(mafScriptTypeKey, QVariant(mafScriptSourceTypeStringScript));
    dic->insert(mafScriptInterpreterKey, QVariant(interpreter));
    return dic;
}

} // mafCore namespace

#endif // MAFOBJECT_H
