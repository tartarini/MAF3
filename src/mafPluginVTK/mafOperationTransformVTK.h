/*
 *  mafOperationTransformVTKVTK.h
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 13/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONTRANSFORMVTK_H
#define MAFOPERATIONTRANSFORMVTK_H

#include "mafPluginVTKDefinitions.h"
#include <mafOperationTransform.h>

namespace mafPluginVTK {

/**
  Class Name: mafOperationTransformVTK
  This operation allow to transform the pose matrix of the input selected VME.
  */
    class MAFPLUGINVTKSHARED_EXPORT mafOperationTransformVTK : public mafResources::mafOperationTransform {
    Q_OBJECT

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperationTransform);

public slots:

public:
    /// Object constructor.
    mafOperationTransformVTK(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
        
    /// Initialize the operation. Put here the initialization of operation's parameters.
    /*virtual*/ bool initialize();

    
protected:
    /// Object destructor.
    /* virtual */ ~mafOperationTransformVTK();

    /// Terminate the operation's execution.
    /*virtual*/ void terminated();

private:
};

} // namespace mafResources

#endif // MAFOPERATIONTRANSFORMVTK_H
