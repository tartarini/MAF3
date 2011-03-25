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

    /// this method de-serialize the member variables from the given stream.
    mafMementoPropertyList *mementoPropertyList();

    /// Return the class type of the object associated.
    QString objectClassType() const;

    /// Assign the object class type from the hinerited mementos.
    void setObjectClassType (const QString &class_type);

protected:
    /// Object destructor.
    /* virtual */ ~mafMemento();

private:
    friend class mafObject;
    QString m_ObjectClassType; ///< Store the object's class type.
    mafMementoPropertyList m_MementoPropertyList; ///< Vector containing the object status.
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

} // namespace mafCore

#endif // MAFMEMENTOOBJECT_H
