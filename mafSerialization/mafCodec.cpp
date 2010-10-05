/*
 *  mafCodec.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodec.h"

using namespace mafCore;
using namespace mafSerialization;

mafCodec::mafCodec(const mafString code_location) : mafObjectBase(code_location), m_EncodingType(""), m_Device(NULL), m_Level(0) {
}

