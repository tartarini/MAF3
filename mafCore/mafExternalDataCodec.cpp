/*
 *  mafExternalDataCodec.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafExternalDataCodec.h"

using namespace mafCore;


mafExternalDataCodec::mafExternalDataCodec(const mafString &code_location) : mafObjectBase(code_location), m_ExternalData(NULL), m_StringSize(0) {
}

mafExternalDataCodec::mafExternalDataCodec(mafContainerInterface *data, const mafString &code_location) : mafObjectBase(code_location), m_ExternalData(data), m_StringSize(0) {
}

mafExternalDataCodec::~mafExternalDataCodec() {

}
