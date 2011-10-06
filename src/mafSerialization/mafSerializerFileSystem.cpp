/*
 *  mafSerializerFileSystem.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSerializerFileSystem.h"

using namespace mafSerialization;

mafSerializerFileSystem::mafSerializerFileSystem(const QString code_location) : mafSerializer(code_location), m_File(NULL) {
}

mafSerializerFileSystem::~mafSerializerFileSystem() {
    delete m_File;
}

void mafSerializerFileSystem::openDevice(mafSerializerOpenMode mode) {
    // Check the existance of the IO device
    if(m_IODevice == NULL) {
        m_IODevice = new QFile();
        m_File = (QFile *)m_IODevice;
    }
    // Assign the URL to it
    QString f = dataURL().toLocalFile();
    m_File->setFileName(f);

    // Create the connection with the media; in this case open the file on disk.
    QIODevice::OpenMode open_mode = (mode == mafSerializerOpenModeIn) ? QIODevice::ReadOnly : QIODevice::WriteOnly /*QIODevice::Append*/;
    bool file_opened = m_File->open(open_mode);

    if(!file_opened) {
        QByteArray ba = mafTr("Not able to open file '%1'").arg(dataURL().toString()).toAscii();
        qCritical("%s", ba.data());
    }
}

void mafSerializerFileSystem::closeDevice() {
    REQUIRE(m_IODevice != NULL);

    m_File->close();
}
