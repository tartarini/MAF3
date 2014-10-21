/*
 *  mafMonitorRAM.cpp
 *  mafCore
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMonitorRAM.h"

#if defined(_WIN32) || defined(WIN32)
    #include "windows.h"
#else
    #ifdef __APPLE__
        #include <sys/types.h>
        #include <sys/sysctl.h>

        #include <mach/vm_statistics.h>
        #include <mach/mach_types.h>
        #include <mach/mach_init.h>
        #include <mach/mach_host.h>

    #else
        #include "sys/types.h"
        #include "sys/sysinfo.h"
    #endif
#endif

using namespace mafCore;

mafMonitorRAM::mafMonitorRAM(const QString code_location) : mafMonitorMemory(code_location) {
}

mafMonitorRAM::~mafMonitorRAM() {
}

void mafMonitorRAM::update() {
#if defined(_WIN32) || defined(WIN32)
    MEMORYSTATUS memoryStatus;
    GlobalMemoryStatus(&memoryStatus);

    m_FreeSpace = memoryStatus.dwAvailPhys / 1048576;
    m_TotalSpace = memoryStatus.dwTotalPhys / 1048576;
#else
    #ifdef __APPLE__
    // from sysctl.h
    // HW_MEMSIZE   24              uint64_t: physical ram size
    int mib[2];
    uint64_t memsize;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE; /*uint64_t: physical ram size */
    len = sizeof(memsize);
    sysctl(mib, 2, &memsize, &len, NULL, 0);
    m_TotalSpace = memsize;

    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO,
                                        (host_info_t)&vm_stats, &count)) {
        m_FreeSpace = (int64_t)vm_stats.free_count * (int64_t)page_size;

//        uint64_t used_memory = ((int64_t)vm_stats.active_count +
//                       (int64_t)vm_stats.inactive_count +
//                       (int64_t)vm_stats.wire_count) *  (int64_t)page_size;

    }
    #else
        struct sysinfo memInfo;
        sysinfo (&memInfo);
        m_TotalSpace = memInfo.totalram;
        m_FreeSpace = memInfo.freeram;
    #endif
#endif

    m_ResultBuffer = mafTr("RAM Memory monitor results: \n\nFree RAM: ");
    m_ResultBuffer.append(QString::number(freeSpace()));
    m_ResultBuffer.append("\n");
    m_ResultBuffer.append(mafTr("Total RAM: "));
    m_ResultBuffer.append(QString::number(totalSpace()));
    m_ResultBuffer.append("\n");
    m_ResultBuffer.append(mafTr("Used RAM: "));
    m_ResultBuffer.append(QString::number(usedSpace()));
    m_ResultBuffer.append("\n");
}
