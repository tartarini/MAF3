/*
 *  mafMementoVolume.h
 *  mafResources
 *
 *  Created by Yubo Tao on 21/10/11.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTOVOLUME_H
#define MAFMEMENTOVOLUME_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafMemento.h>

namespace mafResources {

// Class forwarding list
class mafVolume;

/**
Class name: mafVolumeMemento
This class defines the MAF3 memento class for storing a mafDataSet state.
 This is used to restore a previous stored DataSet state (undo mechanism or serialization porpouses).
 @sa mafCore::mafMemento mafResources::mafMementoResource
*/
class MAFRESOURCESSHARED_EXPORT mafMementoVolume : public mafCore::mafMemento {
    Q_OBJECT
    //Q_PROPERTY(double timeStamp READ timeStamp WRITE setTimeStamp)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoVolume(const QString code_location = "");

    /// object overloaded constructor.
    mafMementoVolume(const mafCore::mafObject *obj, bool binary = true, const QString code_location = "");

    /// Encode property item.
    /* virtual */ void encodeItem(mafCore::mafMementoPropertyItem *item);

    /// Decode property item.
    /* virtual */ void decodeItem(mafCore::mafMementoPropertyItem *item);

    /// Encode type used to encode/decode data.
    QString encodeType() const { return m_EncodeType; }

protected:
    /// Object destructor.
    /* virtual */ ~mafMementoVolume();

private:
    QString m_EncodeType; ///< Encode type used to encode/decode data.
    mafVolume *m_DataSet; ///< Data set to be stored.
};

} // namespace mafResources

#endif // MAFMEMENTOVOLUME_H