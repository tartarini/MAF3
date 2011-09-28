/*
 *  mafToolVTK.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTK_H
#define MAFTOOLVTK_H

// Includes list
#include "mafPluginVTK_global.h"
#include <mafTool.h>

namespace mafPluginVTK {

/**
 Class name: mafToolVTK
 This is the base class for the MAF3 tools. Tools are used to represent widgets or more in general
 objects that are used to perform task but don't need to be persistent on disk.
 */
class MAFPLUGINVTKSHARED_EXPORT mafToolVTK : public mafResources::mafTool {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafTool);

public:
     /// Object constructor.
    mafToolVTK(const QString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTK();
};

} //namespace mafResources

#endif // MAFTOOL_H
