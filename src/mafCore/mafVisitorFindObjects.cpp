/*
 *  mafVisitorFindObjects.cpp
 *  mafCore
 *
 *  Created by Daniele Giunchi on 16/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisitorFindObjects.h"

using namespace mafCore;

mafVisitorFindObjects::mafVisitorFindObjects(const QString code_location) : mafVisitor(code_location) {
    m_ObjectsList = new mafObjectsList();
}

mafVisitorFindObjects::~mafVisitorFindObjects(){
    delete m_ObjectsList;
}

