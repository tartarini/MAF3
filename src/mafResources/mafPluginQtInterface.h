/*
 *  mafPlugin.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINQTINTERFACE_H
#define MAFPLUGINQTINTERFACE_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

/**
Class name: mafPluginQtInterface
This class provides the way MAF3 extend its objects through plug-ins.
*/
class MAFRESOURCESSHARED_EXPORT mafPluginQtInterface {
    
public:
    /** object destructor. */
    virtual ~mafPluginQtInterface();

    /** retrieve plugin information. */
    virtual mafResources::mafPluginInfo pluginInfo() = 0;
    
    /** register objects in the factory. */
    virtual void registerObjects() = 0;
};

} // mafResources

Q_DECLARE_INTERFACE(mafResources::mafPluginQtInterface, "com.scsitaly.mafPluginInterface")

#endif // MAFPLUGINQTINTERFACE_H
