/*
 *  mafOntology.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFONTOLOGY_H
#define MAFONTOLOGY_H

#include "mafObjectBase.h"
#include "mafHierarchy.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafOntology
 This class defines the MAF3 Ontology.
 @sa mafDictionary mafHierarchy
 */
class MAFCORESHARED_EXPORT mafOntology : public mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafOntology(const QString code_location = "");

    /// Return the dictionary item with the given name. Return NULL if the item has not been found.
    mafDictionary *findItemByName(const QString itemName);

    /// Dictionary template based on CSV text file.
    /** The information stored into the CVS file must be ordered acording to this list of items:
    name, editable, author, value type, note, searchable. multiplicity is readonly property and is
    calculated at runtime depending on the values added to the value list item. Item's value are added
    after the creation of the dictionary and not through the template. The file has no header and
    blank lines will be skipped.
    */
    void setDictionaryTemplate(const QString dictionary_template);

protected:
    /// Object destructor.
    /* virtual */ ~mafOntology();
private:
    mafHierarchy *m_Hierarchy; ///< Hierarchy of dictionary items representing the dictionary structure.
};

} // namespace mafCore

#endif // MAFONTOLOGY_H
