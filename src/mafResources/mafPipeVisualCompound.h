/*
 *  mafPipeVisualCompound.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 08/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEVISUALCOMPOUND_H
#define MAFPIPEVISUALCOMPOUND_H

// Includes list
#include "mafPipeVisual.h"
#include <mafProxyInterface.h>

namespace mafResources {

/**
 Class name: mafPipeVisualCompound
 This is the default visual pipes for mafViewCompound. The class has only the responsibility 
 to keep in sync the visibility flag of the view with data hierarchy.
 */
class MAFRESOURCESSHARED_EXPORT mafPipeVisualCompound : public mafPipeVisual {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
     /// Object constructor.
    mafPipeVisualCompound(const QString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualCompound();

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    virtual void updatePipe(double t = -1);
};

} //namespace mafResources

#endif // MAFPIPEVISUALCOMPOUND_H
