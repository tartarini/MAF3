/*
 *  mafMementoDataSetCollection.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTODATASETCOLLECTION_H
#define MAFMEMENTODATASETCOLLECTION_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafMemento.h>
#include <mafContainerInterface.h>

namespace mafResources {

// Class forwarding list

/**
Class name: mafMementoDataSetCollection
This class defines the MAF3 memento class for storing a mafDataSet state.
 This is used to restore a previous stored DataSet state (undo mechanism or serialization porpouses).
 @sa mafCore::mafMemento mafResources::mafMementoResource
*/
class MAFRESOURCESSHARED_EXPORT mafMementoDataSetCollection : public mafCore::mafMemento {
    Q_OBJECT
    Q_PROPERTY(double timeStamp READ timeStamp WRITE setTimeStamp)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoDataSetCollection(const QString code_location = "");

    /// object overloaded constructor.
    mafMementoDataSetCollection(const mafCore::mafObject *obj, const QString code_location = "");

    /// Encode property item.
    /* virtual */ //void encodeItem(mafCore::mafMementoPropertyItem *item, QString path);

    /// Decode property item.
    /* virtual */ //void decodeItem(mafCore::mafMementoPropertyItem *item, QString path);

    /// Return time stamp.
    double timeStamp() const; 

    /// Set time stamp.
    void setTimeStamp(double time);

protected:
    /// Object destructor.
    /* virtual */ ~mafMementoDataSetCollection();

private:
    friend class mafDataSet;
    int m_DataSizeExported; ///< Size of file to be exported.
    int m_DataSizeImported; ///< Size of file to be imported.
    QString m_EncodeType; ///< Encode type used to encode/decode data.
    mafResources::mafDataSetCollection *m_DataSetCollection; ///< Data set to be stored.
    double m_TimeStamp; ///< Time stamp referred to this mafDataSet.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline double mafMementoDataSetCollection::timeStamp() const {
  return m_TimeStamp;
}

inline void mafMementoDataSetCollection::setTimeStamp(double time) {
  m_TimeStamp = time;
}

} // namespace mafResources

#endif // MAFMEMENTODATASETCOLLECTION_H
