/*
 *  mafMonitorHardDisk.cpp
 *  mafCore
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMonitorHardDisk.h"

#ifdef WIN32
    #include "windows.h"
#else
    #ifdef __APPLE__
        #include <sys/mount.h>
    #else
        #include <sys/vfs.h>
    #endif
#endif

using namespace mafCore;

mafMonitorHardDisk::mafMonitorHardDisk(const QString code_location) : mafMonitorMemory(code_location) {
}

mafMonitorHardDisk::~mafMonitorHardDisk() {
}

void mafMonitorHardDisk::update() {
#ifdef WIN32
    __int64 lpFreeBytesAvaibleToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;

    GetDiskFreeSpaceEx(L"C:\\", (PULARGE_INTEGER)&lpFreeBytesAvaibleToCaller,
                       (PULARGE_INTEGER) &lpTotalNumberOfBytes,
    (PULARGE_INTEGER) &lpTotalNumberOfFreeBytes);

    m_FreeSpace = lpTotalNumberOfFreeBytes;
    m_TotalSpace = lpTotalNumberOfBytes;
#else
//    #ifdef __APPLE__

//    #else
    struct statfs64 s;
    statfs64("/", &s);

    if (s.f_blocks > 0) {
        m_TotalSpace = s.f_blocks * s.f_bsize;
        m_FreeSpace = s.f_bfree * s.f_bsize;
    }
//    #endif
#endif

    m_ResultBuffer = mafTr("Hard Disk Memory monitor results: \n\nFree HD: ");
    m_ResultBuffer.append(QString::number(freeSpace()));
    m_ResultBuffer.append("\n");
    m_ResultBuffer.append(mafTr("Total HD: "));
    m_ResultBuffer.append(QString::number(totalSpace()));
    m_ResultBuffer.append("\n");
    m_ResultBuffer.append(mafTr("Used HD: "));
    m_ResultBuffer.append(QString::number(usedSpace()));
    m_ResultBuffer.append("\n");
}
