/*
 *  mafPluginInterface.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 21/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginInterface.h"

using namespace mafCore;
using namespace mafResources;

mafPluginInterface::mafPluginInterface(const QString &pluginFilename, const QString code_location) : mafObjectBase(code_location), m_RefCount(0), m_Loaded(false) {
} 

mafPluginInterface::mafPluginInterface(const mafPluginInterface &Other, const QString code_location) : mafObjectBase(code_location),  m_RefCount(Other.m_RefCount) {
    ++*m_RefCount; // Increase DLL reference counter
}

mafPluginInterface::~mafPluginInterface() {
}
