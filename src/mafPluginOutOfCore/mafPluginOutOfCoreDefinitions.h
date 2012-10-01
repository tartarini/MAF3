/*
 *  mafPluginCTKDefinitions.h
 *  mafPluginCTK
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINOUTOFCOREDEFINITIONS_H
#define MAFPLUGINOUTOFCOREDEFINITIONS_H

// Includes list
#include "mafPluginOutOfCore_global.h"
#include <mafCoreRegistration.h>
#include <mafResourcesRegistration.h>
#include <mafSerializationDefinitions.h>
#include <mafResourcesDefinitions.h>

namespace mafPluginOutOfCore {

/// typedef to define the data type of the volume
typedef enum {
    mafVolUndefined,
    mafVolUnsignedChar,
    mafVolChar,
    mafVolUnsignedShort,
    mafVolShort,
    mafVolUnsignedInt,
    mafVolInt,
    mafVolFloat,
    mafVolDouble    
} mafDataType;

inline int getByteNum(mafDataType dataType) {
    int byteNum = 0;
    switch(dataType) {
        case mafVolUnsignedChar:
        case mafVolChar:
            byteNum = 1;
            break;
        case mafVolUnsignedShort:
        case mafVolShort:
            byteNum = 2;
            break;
        case mafVolUnsignedInt:
        case mafVolInt:
        case mafVolFloat:
            byteNum = 4;
            break;
        case mafVolDouble:
            byteNum = sizeof(double);
            break;
        default:
            qCritical("Unknown data type.\n");
    }
    return byteNum;
}

} //end namespace



#endif // MAFPLUGINOUTOFCOREDEFINITIONS_H
