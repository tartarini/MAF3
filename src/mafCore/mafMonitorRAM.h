/*
 *  mafMonitorRAM.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMONITORRAM_H
#define MAFMONITORRAM_H

// Includes list
#include "mafMonitorMemory.h"

namespace mafCore {

// Class forwarding list

/**
Class name: mafMonitorRAM
This class defines the monitor for memory (RAM), providing information on space.
*/
class MAFCORESHARED_EXPORT mafMonitorRAM : public mafMonitorMemory {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMonitorMemory);

public:
    /// Object constructor.
    mafMonitorRAM(const QString code_location = "");

    /// update memory variables
    /*virtual*/ void update();

protected:
    /// Object destructor.
    /* virtual */ ~mafMonitorRAM();
};

} // namespace mafCore

#endif // MAFMONITORRAM_H
