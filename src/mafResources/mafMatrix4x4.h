/*
 *  mafMatrix4x4.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMATRIX4x4_H
#define MAFMATRIX4x4_H

#include <QMetaType>
#include <QGenericMatrix>

#include <mafCoreDefinitions.h>

#include "mafResources_global.h"

namespace mafResources {
    
/**
 Class Name: mafMatrix4x4
 mafMatrix4x4 represent the extended matrix inheriting from opencv cv::Mat.
 Added  setToIdentity and modify element.
 */



typedef QGenericMatrix<4,4,qreal> mafMatrix4x4;    
/// Define the pointer to the mafMatrix4x4 needed to be registered with Q_DECLARE_METATYPE macro.
typedef mafMatrix4x4 * mafMatrix4x4Pointer;

} // namespace mafResources

Q_DECLARE_METATYPE(mafResources::mafMatrix4x4Pointer);
Q_DECLARE_METATYPE(mafResources::mafMatrix4x4);

#endif // MAFMATRIX4x4_H