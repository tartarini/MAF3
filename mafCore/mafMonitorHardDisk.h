/*
 *  mafMonitorStorage.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMONITORHARDDISK_H
#define MAFMONITORHARDDISK_H

// Includes list
#include "mafMonitorMemory.h"

namespace mafCore {

// Class forwarding list

/**
Class name: mafMonitorHardDisk
This class defines the monitor for storage device, providing information on space.
*/
class MAFCORESHARED_EXPORT mafMonitorHardDisk : public mafMonitorMemory {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMonitorMemory);

protected:
    /// Object destructor.
    /* virtual */ ~mafMonitorHardDisk();

public:
    /// Object constructor.
    mafMonitorHardDisk(const mafString code_location = "");

    /// update storage variables
    /*virtual*/ void update();

};

} // namespace mafCore

#endif // MAFMONITORHARDDISK_H
