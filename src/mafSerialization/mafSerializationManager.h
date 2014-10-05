/*
 *  mafSerializationManager.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZATIONMANAGER_H
#define MAFSERIALIZATIONMANAGER_H

// Includes list
#include "mafSerialization_global.h"
#include <mafProxyInterface.h>
#include <mafExternalDataCodec.h>

namespace mafSerialization {
    
/**
 Class name: mafSerializationManager
 This singleton provides the facade class for the object serialization mechanism.
 It provides also the creation of 3 new IDs:
 - maf.local.serialization.load
 - maf.local.serialization.save : topic used for save the memento of a serializable object which can be serialized by a codec (XML, RAW ...) in an external file. Generally with a Hierarchical data system all the mementoes will be serialized in a unique file.
 - maf.local.serialization.setIgnoreModified
 - maf.local.serialization.ignoreModified
 - maf.local.serialization.export: topic used for calling an external data codec which encode into a specific file format, defined by the codec itself. The final result will be a new file which contains the internal structure of the custom data.
 - maf.local.serialization.import: topic used for calling an external data codec which decode from a specific file format, defined by the codec itself. The final result will be a memory allocation which contains the custom data structure loaded from the file.
 - maf.local.serialization.update: topic used for calling an external data codec which update / decode from a specific file format, defined by the codec itself. The final result will be a memory allocation wich contains the updated custom data structure loaded from the file.
 - maf.local.serialization.extDataImported
 - maf.local.serialization.mementoLoaded
 related to 'load' and 'save' slots and 'mementoLoaded' signal.
 setIgnoreModified is used in combination with save for ignoring all the "modified flags" of the mementoes.
 */
class MAFSERIALIZATIONSHARED_EXPORT mafSerializationManager : mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

Q_SIGNALS:
    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void save(mafCore::mafMemento *memento, const QString &url, const QString &encode_type = "RAW");

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    mafCore::mafMemento *load(const QString &url, const QString &encode_type);

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void exportData(mafCore::mafProxyInterface *externalData, const QString &url, const QString &encode_type = "RAW");

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    mafCore::mafProxyInterface * importData(const QString &url, const QString &encode_type);

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void updateData(mafCore::mafProxyInterface *externalData, const QString &url, const QString &encode_type = "RAW");

    /// Alert all the observers that a new external data has been loaded from the media.
    void extDataImported(mafCore::mafProxyInterface *data);

       /// Signal needed to register new type of custom codec.
    void plugCodecInModule(const QString &encoding_type, const QString &codecType = "RAW");

    /// Signal needed to register custom type of serializer.
    void plugSerializerInModule(const QString &schema, const QString &serializer_type);
    
    /// Signal needed to set all mementoes modified.
    void setIgnoreModifiedSignal(bool value);
    
    /// signal for ignore modified.
    bool ignoreModifiedSignal();
    
    /// signal for requesting working directory.
    QString requestWorkingDirectorySignal();
    
private Q_SLOTS:
    /// Manage the save for the given object.
    /** The 'url' parameter is used to tell the Manager on which media type store the memento.
    This information is written into the url protocol. The last optional parameter (encode_type) is used to customize
    the memento encoding. The default is the raw memory dump, otherwise pass a string for the custom type like 'VTK', 'STL' and so on.*/
    void saveMemento(mafCore::mafMemento *memento, const QString &url, const QString &encode_type = "RAW");

    /// Manage the load of the memento hierarchy from the given URL and decode it according to the given encoding type.
    /** The Serialization Manager extract the protocol from the URL given and create an instance of the related mafSerializer
    that allows to read data from the physical media. Then the manager create the codec necessary to decode the content read
    and reconstruct the memento hierarchy. When the memento has been created, an event is launched to alert the menegers
    that the new memento is live and has to be reconstructed the correct resource associated to it.*/
    mafCore::mafMemento *loadMemento(const QString &url, const QString &encode_type);

    /// Manage the export for the given object.
    void exportExternalData(mafCore::mafProxyInterface *externalData, const QString &url, const QString &encode_type);

    /// Manage the import of an external data from the given URL and decode it according to the given encoding type.
    mafCore::mafProxyInterface * importExternalData(const QString &url, const QString &encode_type);

    /// Manage the update of an external data from the given URL and decode it according to the required data on demand.
    void updateExternalData(mafCore::mafProxyInterface *externalData, const QString &url, const QString &encode_type);

    /// Plug a new codec into the codec hash for the given object type (encoding_type eg. "VTK") (codec_type eg. mafCodecVTK).
    void plugCodec(const QString &encoding_type, const QString &codecType = "RAW");

    /// Plug a new serializer into the serializer hash for the given schema (schema eg. "file") (serializer_type eg. "mafSerialization::mafSerializerFileSystem").
    void plugSerializer(const QString &schema, const QString &serializer_type);
    
    /// set the global variable m_IgnoreModified active, in order to save independently from their Modified flag
    void setIgnoreModified(bool value);
    
    /// return ignore modified value.
    bool ignoreModified();

    /// return requesting working directory.
    QString requestWorkingDirectory();
    
public:
    /// Return an instance of the Manager
    static mafSerializationManager *instance();
    
    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

protected:
    /// Object destructor.
    /* virtual */ ~mafSerializationManager();

private:
    /// Object constructor.
    /** Here are the plug for the default MAF3 codec and serializer.*/
    mafSerializationManager(const QString code_location = "");

    /// Initialize the signals / slots connection and generate the IDs needed for the communication with the Serialization Module.
    void initializeConnections();

    mafCodecHash m_CodecHash; ///< Hash table that store the binding between encoding types (eg. "VTK") and related codec types (eg. mafCodecVTK).
    mafSerializerHash m_SerializerHash; ///< Hash table that store the binding between URL schema and serializer type.
    QString m_ExtDataImportedId; ///< ID related to the signal 'maf.local.serialization.extDataImported' invoked when new external data has been created during load operation.
    mafCore::mafExternalDataCodec *m_CurrentExternalDataCodec; ///< contains the instance of the last created external data codec.
    bool m_IgnoreModified; ///< global variable which is used for determine if all the mementoes modified flag needa to be ignored.

    QString m_WorkingDirectory; ///< global variable for point out in which directory all the session files will be saved.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafSerializationManager::plugSerializer(const QString &schema, const QString &serializer_type) {
    m_SerializerHash.insert(schema, serializer_type);
}

inline bool mafSerializationManager::ignoreModified() {
    return m_IgnoreModified;
}

} // mafSerialization

#endif // MAFSERIALIZATIONMANAGER_H
