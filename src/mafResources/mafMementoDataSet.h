/*
 *  mafMementoDataSet.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTODATASET_H
#define MAFMEMENTODATASET_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafMemento.h>
#include <mafContainerInterface.h>

namespace mafResources {

// Class forwarding list

/**
Class name: mafMementoDataSet
This class defines the MAF3 memento class for storing a mafDataSet state.
 This is used to restore a previous stored DataSet state (undo mechanism or serialization porpouses).
 @sa mafCore::mafMemento mafResources::mafMementoResource
*/
class MAFRESOURCESSHARED_EXPORT mafMementoDataSet : public mafCore::mafMemento {
    Q_OBJECT
    Q_PROPERTY(double timeStamp READ timeStamp WRITE setTimeStamp)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoDataSet(const QString code_location = "");

    /// object overloaded constructor.
    mafMementoDataSet(const mafCore::mafObject *obj, mafPoseMatrix *matrix, mafCore::mafContainerInterface *dataValue, bool binary = true, const QString code_location = "");

    /// Encode property item.
    /* virtual */ void encodeItem(QXmlStreamWriter *XMLStream, QDataStream *binaryStream, QTextStream *textStream, mafCore::mafMementoPropertyItem item);

    /// Decode property item.
    /* virtual */ QVariant decodeItem(QXmlStreamWriter *XMLStream, QDataStream *binaryStream, QTextStream *textStream);

    /// Return time stamp.
    double timeStamp() const; 

    /// Set time stamp.
    void setTimeStamp(double time);

protected:
    /// Object destructor.
    /* virtual */ ~mafMementoDataSet();

private:
    friend class mafDataSet;
    int m_DataSizeExported; ///< Size of file to be exported.
    int m_DataSizeImported; ///< Size of file to be imported.
    QString m_EncodeType; ///< Encode type used to encode/decode data.
    mafResources::mafDataSet *m_DataSet; ///< Data set to be stored.
    double m_TimeStamp; ///< Time stamp referred to this mafDataSet.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline double mafMementoDataSet::timeStamp() const {
  return m_TimeStamp;
}

inline void mafMementoDataSet::setTimeStamp(double time) {
  m_TimeStamp = time;
}

} // namespace mafResources

#endif // MAFMEMENTODATASET_H
