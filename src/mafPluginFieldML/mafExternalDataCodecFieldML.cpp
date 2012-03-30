/*
 *  mafExternalDataCodecFieldML.cpp
 *  mafPluginFieldML
 *
 *  Created by Richad Christie on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafExternalDataCodecFieldML.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginFieldML;

mafExternalDataCodecFieldML::mafExternalDataCodecFieldML(const QString code_location) : mafExternalDataCodec(code_location) {
}

mafExternalDataCodecFieldML::~mafExternalDataCodecFieldML() {
}

char *mafExternalDataCodecFieldML::encode(bool binary) {
    //encode me!

    char *output_string = new char[1];
    if (binary) {
    } else {
    }

    return output_string;
}

void mafExternalDataCodecFieldML::decode(const char *input_string, bool binary) {
    REQUIRE(input_string != NULL);

    // decode me!
}
