/*
 *  mafExternalDataCodecVolume.h
 *  mafSerialization
 *
 *  Created by Yubo Tao on 24/02/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEXTERNALDATACODECVOLUME_H
#define MAFEXTERNALDATACODECVOLUME_H

// Includes list
#include "mafPluginOutOfCoreDefinitions.h"
#include <mafExternalDataCodec.h>
#include <mafProxy.h>

// Class forwarding list

namespace mafPluginOutOfCore {

/**
 * Class name: mafExternalDataCodecVolume
 * Codec to Encode/Decode mafVolume into a raw memory dump using multiresolution.
 * The raw volume stores its data from the highest resolution to the lowest resolution in one large file.
 * In the decode, the memory limit is used to determine which level of the volume data to be loaded.
 * @sa mafVolume, mafExternalDataCodec, mafSerializationManager
 */
class MAFPLUGINOUTOFCORESHARED_EXPORT mafExternalDataCodecVolume : public mafCore::mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    mafExternalDataCodecVolume(const QString code_location = "");

    /// Encode external data into a file directly.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode external data from the file directly.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

protected:
    /// Object destructor.
    ~mafExternalDataCodecVolume();

    /// decode the raw volume data from the indicated level
    /**
     * decode the raw volume data from the indicated level
     * @param startPos           the start positions to be loaded in the current resolution 
     * @param dimensions         the dimensions to be loaded in the current resolution
     * @param level              the multiresolution level to be loaded
     */
    void *decode(int startPos[3], int dimensions[3], int level);
};

} // mafSerialization

#endif // MAFEXTERNALDATACODECVOLUME_H
