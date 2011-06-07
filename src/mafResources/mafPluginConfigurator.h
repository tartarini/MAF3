/*
 *  mafPluginConfigurator.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 06/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINCONFIGURATOR_H
#define MAFPLUGINCONFIGURATOR_H

#include "mafResourcesDefinitions.h"

#include <QDomDocument>

namespace mafResources {

/**
 Class name: mafPluginConfigurator
 This class is the object that is in charge to parse the input XML file and send plug event through the mafEventBus usefull to plug objects into the framework.
 @sa mafEventBusManager, mafPluginManager
 */
class MAFRESOURCESSHARED_EXPORT mafPluginConfigurator {
public:
    /// Object constructor
    mafPluginConfigurator() {};

    /// Object destructor
    ~mafPluginConfigurator() {};

    /// Read the configuration file given as argument and return true on success.
    bool parseConfigurationFile(QString configFile);

private:
    /// Allow to parse the XML configuration file.
    void parseConfigurationFile(QDomNode current);
};

} // namespace mafResources

#endif // MAFPLUGINCONFIGURATOR_H
