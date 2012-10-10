/*
 *  mafMonitorMemory.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMONITORMEMORY_H
#define MAFMONITORMEMORY_H

// Includes list
#include "mafMonitor.h"

namespace mafCore {

// Class forwarding list

/**
Class name: mafMonitorMemory
This class defines the interface class for MAF3 Monitors on Memory Devices, like RAM or Hard Disk.
*/
class MAFCORESHARED_EXPORT mafMonitorMemory : public mafMonitor {
    Q_OBJECT
    Q_PROPERTY(long long Total READ totalSpace)
    Q_PROPERTY(long long Free READ freeSpace)
    Q_PROPERTY(long long Used READ usedSpace)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMonitor);

public:
    /// Object constructor.
    mafMonitorMemory(const QString code_location = "");

    /// return the total space of the device
    virtual long long totalSpace();

    /// return the free space of the device
    virtual long long freeSpace();

    /// update space variables
    /*virtual*/ void update() = 0;

    /// return the total space of the device
    virtual long long usedSpace();

    ///< byte conversion enum
    typedef enum {
        toKiloBytes = 1024,
        toMegaBytes = 1048576,
        toGigaBytes = 1073741824,
    } fromBytesConversion;

    /// Convert actual space value from bytes in Mega Bytes or Giga Bytes.
    static unsigned int conversionFromBytes(long long bytes, int byteConversion = -1);

protected:
    /// Object destructor.
    /* virtual */ ~mafMonitorMemory();
    long long m_TotalSpace; ///< total space on device
    long long m_FreeSpace; ///< free space on device
};

//////////////////
// Inline Methods
//////////////////

inline long long mafMonitorMemory::totalSpace() {
    return m_TotalSpace;
}

inline long long mafMonitorMemory::freeSpace() {
    return m_FreeSpace;
}

inline long long mafMonitorMemory::usedSpace() {
    return m_TotalSpace - m_FreeSpace;
}

} // namespace mafCore

#endif // MAFMONITORMEMORY_H
