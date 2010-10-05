/*
 *  mafDataPipe.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataPipe.h"
#include "mafDataSet.h"

using namespace mafResources;

mafDataPipe::mafDataPipe(const mafString code_location) : mafPipe(code_location), m_DecoratorPipe(NULL), m_Output(NULL) {
}

mafDataPipe::~mafDataPipe() {
    mafDEL(m_Output);
}

