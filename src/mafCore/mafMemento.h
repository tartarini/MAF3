/*
 *  mafMemento.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
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
 This can be used to restore a previous stored object state (for undo mechanism or serialization porpouses).
 */
class MAFCORESHARED_EXPORT mafMemento : public mafObjectBase {
    Q_ENUMS(MementoHierarchyType)
    Q_PROPERTY(MementoHierarchyType mementoHierarchyType READ mementoHierarchyType WRITE setMementoHierarchyType)
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// mafMemento constructor.
    mafMemento(const QString code_location = "");

    /// mafMemento constructor.
    mafMemento(const mafObject *obj, const QString code_location = "");

    ///< Enumeration that define type of memento.
    enum MementoHierarchyType {
      INHERIT_MEMENTO = 0,
      COMPOSE_MEMENTO
    };
    

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Encode property item.
     virtual void encodeItem(mafMementoPropertyItem *item, QString path);

    /// Decode property item.
     virtual void decodeItem(mafMementoPropertyItem *item, QString path);

    /// this method de-serialize the member variables from the given stream.
    mafMementoPropertyList *mementoPropertyList();

    /// Return the class type of the object associated.
    QString objectClassType() const;

    /// Assign the object class type from the inherited mementos.
    void setObjectClassType(const QString &class_type);

    /// Set the type of memento (INHERIT or COMPOSE).
    void setMementoHierarchyType(MementoHierarchyType memType);

    /// Return the type of memento (INHERIT or COMPOSE).
    MementoHierarchyType mementoHierarchyType() const;

protected:
    /// Object destructor.
    /* virtual */ ~mafMemento();

private:
    friend class mafObject;
    QString m_ObjectClassType; ///< Store the object's class type.
    mafMementoPropertyList m_MementoPropertyList; ///< Vector containing the object status.
    MementoHierarchyType m_MementoHierarchyType; ///Enum to determinate if memento is of type INHERIT or COMPOSE.
};
Q_DECLARE_METATYPE(mafMemento::MementoHierarchyType)


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

inline void mafMemento::setMementoHierarchyType(MementoHierarchyType memType) {
  m_MementoHierarchyType = memType;
}

inline mafMemento::MementoHierarchyType mafMemento::mementoHierarchyType() const {
  return m_MementoHierarchyType;
}


} // namespace mafCore

#endif // MAFMEMENTOOBJECT_H
