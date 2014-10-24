/*
 *  mafCoreDefinitions.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDEFINITIONS_H
#define MAFDEFINITIONS_H

// Includes list
#include "mafUtilities.h"
#include "mafCoreConfig.h"

// Qt includes
#include <QtCore>

#define CALLBACK_SIGNATURE "1"
#define SIGNAL_SIGNATURE   "2"

#define mafArgument(var_type, var) QArgument<var_type >(#var_type, var)
#define mafReturnArgument(var_type, var) QReturnArgument<var_type >(#var_type, var)
typedef QList<QGenericArgument> mafArgumentList;

namespace mafCore {

#define mafCodeLocation "\0" __FILE__ ":" QT_STRINGIFY(__LINE__)

#ifdef __FUNCTION__
#define __DBG_FUNCTION__ __FUNCTION__
#else
#define __DBG_FUNCTION__
#endif

#define _DEBUG_PRINT qDebug() <<
#define _DEBUG_MSG "[+] " __FILE__ " " __DBG_FUNCTION__ ":" QT_STRINGIFY(__LINE__) " "
#define DEBUG_VAR(variable) _DEBUG_PRINT _DEBUG_MSG << variable << "\n";
#define DEBUG_LINE _DEBUG_MSG##"\n";
#define DEBUG_SEPARATOR(separator) \
	{ \
	QString sep; \
	for(int i=0;i<20;++i) { \
	sep.append(#separator); \
	} \
	qDebug() << sep; \
} 

#define DEBUG_VAR_FILE(filename, var) { \
	                                  QFile debugLog(filename); \
									  if (!debugLog.open(QIODevice::Append | QIODevice::Text)) { } \
									  else { \
	                                         QTextStream out(&debugLog); \
											 out << _DEBUG_MSG << #var << " -> "<< var << "\n"; \
									         debugLog.close(); \
                                           } \
									  }

typedef long mafId;
typedef QtMessageHandler mafMsgHandlingFunction;

#define mafDateTimeLogFormat "dd-MMM-yyyy hh:mm:ss"

#define mafTr QObject::tr
#define mafSuperclassMacro(superclass) \
    protected: \
    typedef superclass Superclass;

#define mafProxyPointerTypeCast(dataType, proxyInterfacePointer) \
    static_cast<mafCore::mafProxy<dataType> *>(proxyInterfacePointer)
    
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
#define PRINT_FUNCTION_NAME_INFORMATION qDebug() << __FILE__ << ": "<< __FUNCTION__;

/// Enumeration that define the status of the object.
enum mafObjectLock {
    mafObjectLockNone = 0x1,
    mafObjectLockRead = 0x2,
    mafObjectLockWrite = 0x4,
    mafObjectLockProgress = 0x8
};

   
/// Typedef representing an item property of the memento that has to be encoded or decoded.
typedef struct {
    // Encoding variables
    QString m_Name; ///< Name for the encoded property.
    QVariant m_Value; ///< Value for the encoded property.
    int m_Multiplicity; ///< Multiplicity for the encoded property.
} mafMementoPropertyItem;

/// Structure that defines the information used by a plug-in that needs to plug new objects into MAF3 framework.
struct mafPluggedObjectInformation{
    mafPluggedObjectInformation() : m_Label(""), m_ClassType("") {}
    mafPluggedObjectInformation(QString label, QString classtype) : m_Label(label), m_ClassType(classtype) {}
    QString m_Label;      ///< String label associated to the plugged object; it will be shown in the user interface if no custom label is assigned by the user.
    QString m_ClassType;  ///< Class type of the plugged object.
};

///< Enum that define the Serialization pattern. For "Composition" or for "Inheritance"
typedef enum {
  mafSerializationPatternInheritance = 0,
  mafSerializationPatternComposition,
} mafSerializationPattern;

///< Enum that define the script type contained into the QVariantHash passed to the mafObject.
typedef enum mafScriptSourceType {
    mafScriptSourceTypeFilePath = 0,
    mafScriptSourceTypeStringScript
} mafScriptSourceType;

#define mafScriptKey "script"
#define mafScriptTypeKey "type"
#define mafScriptInterpreterKey "interpreter"

/// Typedef that defines the list of information related to the same extended base MAF class type. It is used through the mafPluggedObjectsHash defined in mafPluginManager.
typedef QList<mafPluggedObjectInformation> mafPluggedObjectInformationList;

/// Hash of plugged objects coming from plug-ins.
typedef QHash<QString, mafPluggedObjectInformation> mafPluggedObjectsHash;

/// Define the property list vector that hold properties to be serialized through mafCodec.
typedef QVector<mafMementoPropertyItem> mafMementoPropertyList;

class mafObjectBase;
/// Define type associated to the list of objects stored into the mafObjectRegistry and usable to define a list of mafObjectBase generically.
typedef QList<mafObjectBase *> mafObjectsList;

}  // namespace mafCore

#endif // MAFDEFINITIONS_H

#include "mafContracts.h"
