/*
 *  mafMonitorMemory.cpp
 *  mafCore
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMonitorMemory.h"

using namespace mafCore;

mafMonitorMemory::mafMonitorMemory(const QString code_location) : mafMonitor(code_location), m_TotalSpace(0), m_FreeSpace(0) {
}

mafMonitorMemory::~mafMonitorMemory() {
}

unsigned int mafMonitorMemory::conversionFromBytes(long long bytes, int byteConversion) {

    return bytes / byteConversion;
}
