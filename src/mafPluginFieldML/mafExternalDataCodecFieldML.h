/*
 *  mafExternalDataCodecFieldML.h
 *  mafPluginFieldML
 *
 *  Created by Richard Christie on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEXTERNALDATACODECFIELDML_H
#define MAFEXTERNALDATACODECFIELDML_H

// Includes list
#include "mafPluginFieldMLDefinitions.h"
#include <mafExternalDataCodec.h>
#include <mafProxy.h>

// Class forwarding list

namespace mafPluginFieldML {

/**
 Class name: mafExternalDataCodecFieldML
 Codec to Encode/Decode MAF objects into a FieldML memory dump.
 */
class MAFPLUGINFIELDMLSHARED_EXPORT mafExternalDataCodecFieldML : public mafCore::mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    mafExternalDataCodecFieldML(const QString code_location = "");

    /// Encode external data into a QDataStream.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode external data from the QDataStream.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

protected:
    /// Object destructor.
    ~mafExternalDataCodecFieldML();

private:
    //vtkDataSetReader *m_Reader; ///< Reader of the vtkDataSet
    //mafCore::mafProxy<vtkAlgorithmOutput> *m_Data; ///< External data type do be coded/encoded.
};

} // mafPluginVTK

#endif // MAFEXTERNALDATACODECVTK_H
