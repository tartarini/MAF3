/*
 *  mafExternalDataCodecVTK.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEXTERNALDATACODECVTK_H
#define MAFEXTERNALDATACODECVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafExternalDataCodec.h>
#include <mafContainer.h>

// Class forwarding list
class vtkAlgorithmOutput;
class vtkDataSetReader;

namespace mafPluginVTK {

/**
 Class name: mafExternalDataCodecVTK
 Codec to Encode/Decode maf objects into a VTK memory dump.
 */
class MAFPLUGINVTKSHARED_EXPORT mafExternalDataCodecVTK : public mafCore::mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    mafExternalDataCodecVTK(const mafString code_location = "");

    /// Object destructor.
    ~mafExternalDataCodecVTK();

    /// Encode external data into a mafDataStream.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode external data from the mafDataStream.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

private:
    vtkDataSetReader *m_Reader; ///< Reader of the vtkDataSet
    mafCore::mafContainer<vtkAlgorithmOutput> *m_Data; ///< External data type do be coded/encoded.
};

} // mafPluginVTK

#endif // MAFEXTERNALDATACODECVTK_H
