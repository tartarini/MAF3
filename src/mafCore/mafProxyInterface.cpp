/*
 *  mafProxyInterface.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafProxyInterface.h"

using namespace mafCore;

mafProxyInterface::mafProxyInterface() : m_ExternalDataType(""), m_ExternalCodecType("") {
}

mafProxyInterface::mafProxyInterface(const mafProxyInterface &dw) {
    *this = dw;
}

bool mafProxyInterface::isEqual(mafProxyInterface *container) {
    Q_UNUSED(container);
    return false;
}
