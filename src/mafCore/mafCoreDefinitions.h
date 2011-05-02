/*
 *  mafCoreDefinitions.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDEFINITIONS_H
#define MAFDEFINITIONS_H

// Includes list
#include "mafUtilities.h"

// Qt includes
#include <QtCore>

#define CALLBACK_SIGNATURE "1"
#define SIGNAL_SIGNATURE   "2"

namespace mafCore {

#define mafCodeLocation __FILE__":"QTOSTRING(__LINE__)

typedef long mafId;
typedef QtMsgHandler mafMsgHandlingFunction;

#define mafDateTimeLogFormat "dd-MMM-yyyy hh:mm:ss"

#define mafTr QObject::tr
#define mafSuperclassMacro(superclass) \
    protected: \
    typedef superclass Superclass;

#define mafContainerPointerTypeCast(dataType, containerInterfacePointer) \
    static_cast<mafContainer<dataType> *>(containerInterfacePointer)
    
/// typedef that represents dictionary entries ( key  , value )
typedef QHash<QString, QVariant> mafDictionary;
typedef mafDictionary * mafDictionaryPointer;


///< Enum that identify the mafLogMode's type.
typedef enum {
    mafLogModeAllMessages = 0,
    mafLogModeNoWarnings,
    mafLogModeNoDebug,
    mafLogModeOnlyCritical,
    mafLogModeOnlyFatal,
    mafLogModeTestSuite,
    mafLogModeCustom
} mafLogMode;

#define TEST_SUITE_LOG_PREFIX " -- TestSuite Message -- "

/// Enumeration that define the status of the object.
enum mafObjectLock {
    mafObjectLockNone = 0x0,
    mafObjectLockRead = 0x1,
    mafObjectLockWrite = 0x2,
    mafObjectLockProgres = 0x4
                       };
///< Enumeration that define type of memento.
enum MementoHierarchyType {
    INHERIT_MEMENTO = 0,
    COMPOSE_MEMENTO
};
    
/// Typedef representing an item property of the memento that has to be encoded or decoded.
typedef struct {
    // Encoding valiables
    QString m_Name; ///< Name for the encoded property.
    QVariant m_Value; ///< Value for the encoded property.
    int m_Multiplicity; ///< Multiplicity for the encoded property.
} mafMementoPropertyItem;

/// Struct that defines the information used by a plug-in that needs to plug new objects into MAF3 framework.
struct mafPluggedObjectInformation{
    mafPluggedObjectInformation() : m_Label(""), m_ClassType("") {}
    mafPluggedObjectInformation(QString label, QString classtype) : m_Label(label), m_ClassType(classtype) {}
    QString m_Label;      ///< String label associated to the plugged object; it will be shown in the user interface if no custom label is assigned by the user.
    QString m_ClassType;  ///< Class type of the plugged object.
};

/// Typedef that defines the list of information related to the same extended base MAF class type. It is used through the mafPluggedObjectsHash defined in mafPluginManager.
typedef QList<mafPluggedObjectInformation> mafPluggedObjectInformationList;

/// Hash of plugged objects coming from plig-ins.
typedef QHash<QString, mafPluggedObjectInformation> mafPluggedObjectsHash;

/// Define the property list vector that hold properties to be serialized through mafCodec.
typedef QVector<mafMementoPropertyItem> mafMementoPropertyList;

class mafObjectBase;
/// Define type associated to the list of objects stored into the mafObjectRegistry and usable to define a list of mafObjectBase generically.
typedef QList<mafObjectBase *> mafObjectsList;

}  // mafCore

Q_DECLARE_FLAGS(mafObjectLockStatus, mafCore::mafObjectLock);
Q_DECLARE_METATYPE(mafCore::mafDictionaryPointer);
Q_DECLARE_METATYPE(mafCore::MementoHierarchyType)

#endif // MAFDEFINITIONS_H

#include "mafContracts.h"
