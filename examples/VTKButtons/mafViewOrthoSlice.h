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

#include <mafView.h>
 
/**
 Class name: mafViewOrthoSlice
 mafView compound with 4 panels containing 4 rendering windows.
*/
class mafViewOrthoSlice : public mafResources::mafView {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafView);
    
public:
    /// Object constructor.
    mafViewOrthoSlice(const QString code_location = "");
    
    /// Create the view
    /*virtual*/ bool initialize();

protected:
    /// Object destructor.
    /* virtual */ ~mafViewOrthoSlice();

Q_SIGNALS:
    /// Signal used to propagate the mafSceneNode visibility.
    void propagateSceneNodeShowSignale();
};
