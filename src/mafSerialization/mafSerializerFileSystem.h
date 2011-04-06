/*
 *  mafSerializerFileSystem.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZERFILESYSTEM_H
#define MAFSERIALIZERFILESYSTEM_H

// Includes list
#include "mafSerialization_global.h"
#include "mafSerializer.h"

namespace mafSerialization {

/**
 Class name: mafSerializerFileSystem
 This class is responsible to write input DataStream into a file based on local filesystem.
 @sa mafSerializer
 */
class MAFSERIALIZATIONSHARED_EXPORT mafSerializerFileSystem : public mafSerializer {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafSerializer);

public:
    /// Object constructor.
    mafSerializerFileSystem(const QString code_location = "");

    /// Initialize the IO device and make it ready to operate.
    /*virtual*/ void openDevice(mafSerializerOpenMode mode);

    /// Close the IO device and flush all the information into the media.
    /*virtual*/ void closeDevice();

protected:
    /// Object destructor.
    /* virtual */ ~mafSerializerFileSystem();
private:
    QFile *m_File; ///< Reference to the file media.

}; // class mafSerializerFileSystem

} // namespace mafSerialization

#endif // MAFSERIALIZERFILESYSTEM_H
