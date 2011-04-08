/*
 *  mafVisitorHierarchyInformation.cpp
 *  mafCore
 *
 *  Created by Daniele Giunchi on 16/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisitorHierarchyInformation.h"

using namespace mafCore;

mafVisitorHierarchyInformation::mafVisitorHierarchyInformation(const QString code_location) : mafVisitor(code_location) {
    m_ObjectsList = new mafObjectsList();
}

mafVisitorHierarchyInformation::~mafVisitorHierarchyInformation(){
    delete m_ObjectsList;
}

