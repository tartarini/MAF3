/*
 *  mafObject.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFOBJECT_H
#define MAFOBJECT_H

// Includes list
#include "mafObjectBase.h"

namespace mafCore {

// Class forwarding list
class mafMemento;


/**
 Class name: mafObject
 This class defines the basic API for objects that wants to be serialized .
 */
class MAFCORESHARED_EXPORT mafObject : public mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(QString uiFile READ uiFilename)

    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafObject(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafObject();

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

    /// Return the instance of the object's status. The caller has to delete the allocated memento memory he asked for.
    virtual mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization pourposes.*/
    virtual void setMemento(mafMemento *memento, bool deep_memento = false);
    
    /// Add string tag to the tag list, that will categorize the object.
    /** This list of tags wants to define a series of strings that describe the object and
    categorize it in the cloud of MAF3 objects (like a photo tags on Flickr).
    This method return true if the tag is added correctly, otherwise false.
    It return false also if you are trying to add and already added tag.*/
    bool addTag(mafString tag);

    /// Remove from the tag list the given string tag.
    bool removeTag(mafString tag);

    /// Find all the tags that match the given string patter and put the result into the 'filtered_list argument.
    void filterTags(mafString tag_pattern, mafTagList *filtered_list);

    /// Return the list of all tag inserted.
    const mafTagList *tagList() const;

    /// Return the filename associated to the object's UI.
    const mafString uiFilename() const;

    /// Return the dictionary associated to the object.
    mafDictionary *dictionary() const;

public slots:
    /// Method used to initialize the object's tag list.
    void setTagList(const mafTagList &list);

protected:
    mafString m_UIFilename; ///< Filename that define the object's UI written into a XML file.

private:
    mafTagList *m_TagList; ///< Tag list that categorize the object.
    mafDictionary* m_Dictionary; ///< Dictionary associated to the object.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline const mafTagList *mafObject::tagList() const {
    return m_TagList;
}

inline const mafString mafObject::uiFilename() const {
    return m_UIFilename;
}

inline mafDictionary *mafObject::dictionary() const {
    return m_Dictionary;
}

} // mafCore namespace

#endif // MAFOBJECT_H
