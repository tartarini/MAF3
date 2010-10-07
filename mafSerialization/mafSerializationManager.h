/*
 *  mafSerializationManager.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZATIONMANAGER_H
#define MAFSERIALIZATIONMANAGER_H

// Includes list
#include "mafSerialization_global.h"
#include <mafContainerInterface.h>



// Class forwarding list
class mafContainerInterface;


namespace mafSerialization {

/**
 Class name: mafSerializationManager
 This singletone provides the facade class for the object serialization mechanism.
 It provides also the creation of 3 new IDs:
 - maf.local.serialization.load
 - maf.local.serialization.save
 - maf.local.serialization.mementoLoaded
 related to 'load' and 'save' slots and 'mementoLoaded' signal.
 */
class MAFSERIALIZATIONSHARED_EXPORT mafSerializationManager : mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafSerializationManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

signals:
    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void save(mafCore::mafMemento *memento, const mafString &url, const mafString &encode_type = "RAW");

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void load(const mafString &url, const mafString &encode_type);

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void exportData(mafCore::mafContainerInterface *externalData, const mafString &url, const mafString &encode_type = "RAW");

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void importData(const mafString &url, const mafString &encode_type);

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    void saveExtData(char *externalDataString, const mafString &url, int stringSize);

    /// Signals that defines the API for the serialization manager. These will call private slots to do the right work.
    mafByteArray loadExtData(const mafString &url);

    /// Alert all the observers that a new memento has been loaded from the media.
    void mementoLoaded(mafCore::mafMemento *m);

    /// Alert all the observers that a new external data has been loaded from the media.
    void extDataImported(mafCore::mafContainerInterface *data);

       /// Signal needed to register new type of custom codec.
    void plugCodecInModule(const mafString &object_type, const mafString &encoding_type, const mafString &codecType = "RAW");

    /// Signal needed to register custom type of serializer.
    void plugSerializerInModule(const mafString &schema, const mafString &serializer_type);

private slots:
    /// Manage the save for the given object.
    /** The 'url' parameter is used to tell the Manager on which media type store the memento.
    This information is written into the url protocol. The last optional parameter (encode_type) is used to customize
    the memento encoding. The default is the raw memory dump, otherwise pass a string for the custom type like 'VTK', 'STL' and so on.*/
    void saveMemento(mafCore::mafMemento *memento, const mafString &url, const mafString &encode_type = "RAW");

    /// Manage the load of the memento hierarchy from the given URL and decode it according to the given encoding type.
    /** The Serialization Manager extract the protocol from the URL given and create an instance of the related mafSerializer
    that allows to read data from the physical media. Then the manager create the codec necessary to decode the content read
    and reconstruct the memento hierarchy. When the memento has been created, an event is launched to alert the menegers
    that the new memento is live and has to be reconstructed the correct resource associated to it.*/
    void loadMemento(const mafString &url, const mafString &encode_type);

    /// Manage the export for the given object.
    void exportExternalData(mafCore::mafContainerInterface *externalData, const mafString &url, const mafString &encode_type);

    /// Manage the import of an external data from the given URL and decode it according to the given encoding type.
    void importExternalData(const mafString &url, const mafString &encode_type);

    /// Manage the load of an external data from the given URL and decode it according to the given encoding type.
    mafByteArray loadExternalData(const mafString &url);

    /// Manage the save for the given object.
    void saveExternalData(char *externalDataString, const mafString &url, int stringSize);

    /// Return the list of encoding type (RAW, VTK, STL...) associated to the given memento.
    mafEncodingList encodingTypeList(const mafCore::mafMemento *memento = NULL);

    /// Plug a new codec into the codec hash for the given object type (object_type eg. mafResources::mafVMESurface) (encoding_type eg. "VTK") (codec_type eg. mafCodecVTK).
    void plugCodec(const mafString &object_type, const mafString &encoding_type, const mafString &codecType = "RAW");

    /// Plug a new serializer into the serializer hash for the given schema (schema eg. "file") (serializer_type eg. "mafSerialization::mafSerializerFileSystem").
    void plugSerializer(const mafString &schema, const mafString &serializer_type);

private:
    /// Object constructor.
    /** Here are the plug for the default MAF3 codec and serializer.*/
    mafSerializationManager(const mafString code_location = "");

    /// Object destructor.
    ~mafSerializationManager() {}

    /// Initialize the signals / slots connection and generate the IDs needed for the communication with the Serialization Module.
    void initializeConnections();

    mafEncodingHash m_EncodingHash; ///< Hash table that store the binding between objetc types and related encoding types.
    mafCodecHash m_CodecHash; ///< Hash table that store the binding between encoding types (eg. "VTK") and related codec types (eg. mafCodecVTK).
    mafSerializerHash m_SerializerHash; ///< Hash table that store the binding between URL schema and serializer type.

    mafString m_MementoLoadedId; ///< ID related to the signal 'maf.local.serialization.mementoLoaded' invoked when new memento has been created during load operation.

    mafString m_ExtDataImportedId; ///< ID related to the signal 'maf.local.serialization.extDataImported' invoked when new external data has been created during load operation.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafSerializationManager::plugSerializer(const QString &schema, const QString &serializer_type) {
    m_SerializerHash.insert(schema, serializer_type);
}

} // mafSerialization

#endif // MAFSERIALIZATIONMANAGER_H
