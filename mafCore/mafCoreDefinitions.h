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
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QDataStream>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QBuffer>
#include <QHash>
#include <QTime>
#include <QLinkedList>
#include <QThread>
#include <QThreadPool>
#include <QFuture>
#include <QObject>
#include <QUuid>
#include <QMetaProperty>
#include <qtconcurrentrun.h>
#include <QLibrary>
#include <QProcess>
#include <QSettings>

namespace mafCore {

#define mafCodeLocation __FILE__":"QTOSTRING(__LINE__)

typedef long mafId;
typedef QtMsgHandler mafMsgHandlingFunction;
#define mafMsgType QtMsgType
#define mafString QString
#define mafChar QChar
#define mafByteArray QByteArray
#define mafStringList QStringList
#define mafVariant QVariant
#define mafVariantList QVariantList
#define mafUuid QUuid
#define mafSettings QSettings

#define mafDataStream QDataStream
#define mafTextStream QTextStream
#define mafFile QFile
#define mafBuffer QBuffer

#define mafList QList
#define mafLinkedList QLinkedList
#define mafLinkedListIterator QLinkedListIterator
#define mafHash QHash
#define mafMap QMap
#define mafDateTime QDateTime
#define mafDateTimeLogFormat "dd-MMM-yyyy hh:mm:ss"
#define mafTime QTime

#define mafTagList mafList<mafVariant>

#define mafLibrary QLibrary

#define mafThread QThread
#define mafThreadPool QThreadPool
#define mafConcurrent QtConcurrent
#define mafFuture QFuture

#define mafProcess QProcess

#define mafTr QObject::tr
#define mafSuperclassMacro(superclass) \
    protected: \
    typedef superclass Superclass;

#define mafContainerPointerTypeCast(dataType, containerInterfacePointer) \
    static_cast<mafContainer<dataType> *>(containerInterfacePointer)
    
#define mafMsgTypeDebug QtDebugMsg
#define mafMsgTypeWarning QtWarningMsg
#define mafMsgTypeCritical QtCriticalMsg
#define mafMsgTypeFatal QtFatalMsg
#define mafMsgTypeSystem QtSystemMsg

#define mafMsgDebug qDebug
#define mafMsgFatal qFatal
#define mafMsgCritical qCritical
#define mafMsgWarning qWarning

/// typedef that represents dictionary entries ( key  , value )
typedef mafHash<mafString, mafVariant> mafDictionary;
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

/** Typedef representing an item property of the memento that has to be encoded or decoded.*/
typedef struct {
    // Encoding valiables
    mafString m_Name; ///< Name for the encoded property.
    mafVariant m_Value; ///< Value for the encoded property.
    int m_Multiplicity; ///< Multiplicity for the encoded property.
} mafMementoPropertyItem;

/// Typedef that defines the information used by a plug-in that needs to plug new objects into MAF3 framework.
typedef struct mafPluggedObjectInformation {
    mafPluggedObjectInformation() : m_Label(""), m_ClassType("") {};
    mafPluggedObjectInformation(mafString label, mafString classtype) : m_Label(label), m_ClassType(classtype) {};
    mafString m_Label;      ///< String label associated to the plugged object; it will be shown in the user interface if no custom label is assigned by the user.
    mafString m_ClassType;  ///< Class type of the plugged object.
};

/// Typedef that defines the list of information related to the same extended base MAF class type. It is used through the mafPluggedObjectsHash defined in mafPluginManager.
typedef mafList<mafPluggedObjectInformation> mafPluggedObjectInformationList;

/// Hash of plugged objects coming from plig-ins.
typedef mafHash<mafString, mafPluggedObjectInformation> mafPluggedObjectsHash;

/// Define the property list vector that hold properties to be serialized through mafCodec.
typedef QVector<mafMementoPropertyItem> mafMementoPropertyList;

class mafObjectBase;
/// Define type associated to the list of objects stored into the mafObjectRegistry and usable to define a list of mafObjectBase generically.
typedef mafList<mafObjectBase *> mafObjectsList;

}  // mafCore

Q_DECLARE_METATYPE(mafCore::mafDictionaryPointer);

#endif // MAFDEFINITIONS_H

#include "mafContracts.h"
