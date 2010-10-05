/*
 *  mafCodecRaw.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecRaw.h"
#include <mafEventBusManager.h>
#include "QDir"

using namespace mafCore;
using namespace mafSerialization;

mafCodecRaw::mafCodecRaw(const mafString code_location) : mafCodec(code_location) {
}

mafCodecRaw::~mafCodecRaw() {
}
