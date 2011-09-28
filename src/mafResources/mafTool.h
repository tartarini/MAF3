/*
 *  mafTool.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOL_H
#define MAFTOOL_H

// Includes list
#include "mafPipeVisual.h"

namespace mafResources {

/**
 Class name: mafTool
 This is the base class for the MAF3 tools. Tools are used to represent widgets or more in general
 objects that are used to perform task but don't need to be persistent on disk.
 */
class MAFRESOURCESSHARED_EXPORT mafTool : public mafPipeVisual {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
     /// Object constructor.
    mafTool(const QString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafTool();
};

} //namespace mafResources

#endif // MAFTOOL_H
