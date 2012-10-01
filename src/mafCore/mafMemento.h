/*
 *  mafMemento.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFMEMENTO_H
#define MAFMEMENTO_H

// Includes list
#include "mafObjectBase.h"

namespace mafCore {

// Class forwarding list
class mafObject;

/**
 Class name: mafMementoObject
 This class defines the MAF3 base object for storing a mafObject state.
 This can be used to restore a previous stored object state (for undo mechanism or serialization purpose).
 */
class MAFCORESHARED_EXPORT mafMemento : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// mafMemento constructor.
    mafMemento(const QString code_location = "");

    /// mafMemento constructor.
    mafMemento(const mafObject *obj, const QString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Encode property item.
     virtual void encodeItem(mafMementoPropertyItem *item);

    /// Decode property item.
     virtual void decodeItem(mafMementoPropertyItem *item);

    /// this method de-serialize the member variables from the given stream.
    mafMementoPropertyList *mementoPropertyList();

    /// Return the class type of the object associated.
    QString objectClassType() const;

    /// Assign the object class type from the inherited mementos.
    void setObjectClassType(const QString &class_type);

    /// Return isInheritMemento
    mafSerializationPattern serializationPattern() const;

    /// Set InheritMemento.
    inline void setSerializationPattern(mafSerializationPattern serializationPattern);

protected:
    /// Object destructor.
    /* virtual */ ~mafMemento();

private:
    friend class mafObject;
    QString m_ObjectClassType; ///< Store the object's class type.
    mafMementoPropertyList m_MementoPropertyList; ///< Vector containing the object status.
    mafSerializationPattern m_SerializationPattern; ///< Defines if is an INHERIT or a COMPOSED memento.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafMemento::setObjectClassType(const QString &class_type) {
    m_ObjectClassType = class_type;
}

inline mafMementoPropertyList *mafMemento::mementoPropertyList() {
  return &m_MementoPropertyList;
}

inline QString mafMemento::objectClassType() const {
    return m_ObjectClassType;
}

inline mafSerializationPattern mafMemento::serializationPattern() const {
  return m_SerializationPattern;
}

inline void mafMemento::setSerializationPattern(mafSerializationPattern serializationPattern) {
  m_SerializationPattern = serializationPattern;
}


} // namespace mafCore

#endif // MAFMEMENTOOBJECT_H
