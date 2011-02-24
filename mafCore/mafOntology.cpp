/*
 *  mafOntology.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOntology.h"
#include "mafObjectFactory.h"

using namespace mafCore;

mafOntology::mafOntology(const mafString code_location) : mafObjectBase(code_location), m_Hierarchy(NULL) {
    m_Hierarchy = new mafHierarchy();
}

mafOntology::~mafOntology() {
    // Clean up the memory allocated for the dictionary items.
    m_Hierarchy->clear();
    mafDEL(m_Hierarchy);
}

void mafOntology::setDictionaryTemplate(const mafString dictionary_template) {
    REQUIRE(dictionary_template != "");

    // Split the template file content into rows.
    mafStringList template_items = dictionary_template.split("\n");
    mafString item;
    mafString name, editable, author, value_type, note, searchable;
    foreach(item, template_items) {
        // Extract the information for the dictionary item from the template's row.
        mafStringList items = item.split(",");
        name = items.at(0);
        editable = items.at(1);
        author = items.at(2);
        value_type = items.at(3);
        note = items.at(4);
        searchable = items.at(5);
        // Create a new dictionary item.
        /*mafDictionary *dic_item = mafNEW(mafCore::mafDictionary);
        dic_item->entries()->insert("Name", name);
        dic_item->entries()->insert("Editable", "1");
        dic_item->entries()->insert("Author", author);
        dic_item->entries()->insert("ValueType", value_type);
        dic_item->entries()->insert("Searchable", "1");*/

        // Add the dictionary item to the tree.
        //m_Hierarchy.addHierarchyNode(dic_item);
        // move back the iterator to the root node
        //m_Hierarchy.moveTreeIteratorToParent();
    }
}

mafDictionary *mafOntology::findItemByName(const mafString itemName) {
    Q_UNUSED(itemName);
    REQUIRE(itemName.length() > 0);

    // ToDo: Search through the dictionary item's hierarchy.
    // A visitor pattern can be used in order to browse the data structure
    // searching correct value of a key and/or value inside the dictionary

    //search inside hierarchy
    //search inside dictionary


    return NULL;
}
