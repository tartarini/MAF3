/*
 *  mafContainerInterface.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafContainerInterface.h"

using namespace mafCore;

mafContainerInterface::mafContainerInterface() : m_ExternalDataType(""), m_ExternalCodecType("") {
}

mafContainerInterface::mafContainerInterface(const mafContainerInterface &dw) {
    *this = dw;
}
