/*
 *  mafPluginRegistrator.h
 *  mafPluginQt
 *
 *  Created by Matteo Giacomoni on 03/07/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINQTREGISTRATOR_H
#define MAFPLUGINQTREGISTRATOR_H

#include "mafPluginZipDefinitions.h"

namespace mafPluginZip {

/**
 Class name: mafPluginRegistrator
 This class is the object that is in charge to register all the class contained into the library as plug-in
 by sending the message through the mafEventBus using the macro mafRegisterPluginEvent.
 @sa mafEventBusManager, mafPluginManager
 */
class MAFPLUGINZIPSHARED_EXPORT mafPluginRegistrator : public QObject, mafResources::mafPluginQtInterface {
    Q_OBJECT
    Q_INTERFACES(mafResources::mafPluginQtInterface)
    
public:
    /// Object destructor
    /*virtual*/ ~mafPluginRegistrator();

    /*virtual*/ mafResources::mafPluginInfo pluginInfo();
    
    /// Register all the objects present into the library.
    /*virtual*/ void registerObjects();
};

} // namespace mafPluginTest

#endif // MAFPLUGINQTREGISTRATOR_H
