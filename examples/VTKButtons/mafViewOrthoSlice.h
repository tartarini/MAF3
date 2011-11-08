/*
 *  mafViewOrthoSlice.h
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWORTHOSLICE_H
#define MAFVIEWORTHOSLICE_H

#include <mafViewCompound.h>
 
/**
 Class name: mafViewOrthoSlice
 mafView compound with 4 panels containing 4 rendering windows.
*/
class mafViewOrthoSlice : public mafResources::mafViewCompound {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafViewCompound);
    
public:
    /// Object constructor.
    mafViewOrthoSlice(const QString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafViewOrthoSlice();
};

#endif // MAFVIEWORTHOSLICE_H
