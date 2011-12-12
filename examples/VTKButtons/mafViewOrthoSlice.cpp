/*
 *  mafViewOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewOrthoSlice.h"
#include "mafOrthoSlice.h"


mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafResources::mafViewCompound(code_location) {
	setConfigurationFile("OrthoSlice.xml");
    this->setUIRootWidget(new mafOrthoSlice());
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
}
