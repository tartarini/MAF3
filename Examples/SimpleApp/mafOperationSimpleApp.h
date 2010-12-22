/*
 *  mafOperationSimpleApp.h
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONSIMPLEAPP_H
#define MAFOPERATIONSIMPLEAPP_H

#include <mafOperation.h>

/**
  Class Name: mafOperationSimpleApp
  This class represent a test operation that will be plugged from the vertical application.
  */
class mafOperationSimpleApp : public mafResources::mafOperation {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    mafOperationSimpleApp(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
};

#endif // MAFOPERATIONSIMPLEAPP_H
