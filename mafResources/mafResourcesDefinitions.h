/*
 *  mafResourcesDefinitions.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCESDEFINITIONS_H
#define MAFRESOURCESDEFINITIONS_H

#define degreesToRadiant(x) (M_PI * (x) / (double)180.0)
#define radiantToDegrees(x) ((x) * (double)180.0/M_PI)

// Includes list
#include "mafResources_global.h"
#include <qmath.h>
#include <QUrl>
#include <mafCoreDefinitions.h>
#include <mafEventBusManager.h>
#include <mafTopicRegistry.h>
#include <mafCoreSingletons.h>
#include <mafObject.h>
#include <mafHierarchy.h>
#include <mafContainerInterface.h>


// vnl includes
#include <vnl/vnl_double_4x4.h>
#include <vnl/vnl_vector_fixed.h>


namespace mafResources {

#define mafUrl QUrl

class mafDataSet;
class mafDataSetCollection;
class mafResource;
class mafVME;

/// Type definition for MAF3 pose matrix.
typedef vnl_double_4x4 mafPoseMatrix;

/// Type definition for a list of mafResource.
typedef QList<mafResource *> mafResourceList;

/// Time varying data set map.
typedef QMap<double, mafDataSet *> mafDataSetMap;

/// List of dataset used as input list for mafPipe.
typedef QList<mafDataSet *> mafDataSetList; //to be removed?

/// List of VME used as input list for mafPipe.
typedef QList<mafVME *> mafVMEList;

/** Structure representing the information about a plug-in library.*/
struct MAFRESOURCESSHARED_EXPORT mafPluginInfo {
    float m_Version;            ///< Plug-in Version
    QString m_PluginName;     ///< Plug-in name
    QString m_Author;         ///< Plug-in author
    QString m_Vendor;         ///< Plug-in company name
    QString m_VendorHomepage; ///< Plug-in company homepage
    QString m_Description;    ///< Plug-in general description.
};

/// Hash of plugged objects coming from plig-ins.
//typedef QHash<QString, mafCore::mafPluggedObjectInformation> mafPluggedObjectsHash;

///< Enumeration that define the Cartesian Axis.
enum {
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS
};

/// Enumeration that define the operation type.
typedef enum {
    mafOperationTypeImporter = 0,
    mafOperationTypeExporter,
    mafOperationTypeOperation
} mafOperationType;

}

#endif // MAFRESOURCESDEFINITIONS_H
