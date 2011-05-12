/*
 *  mafDataSetCollection.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataSetCollection.h"
#include "mafInterpolator.h"
#include "mafDataSet.h"
#include "mafMementoDataSetCollection.h"

using namespace mafCore;
using namespace mafResources;

mafDataSetCollection::mafDataSetCollection(const QString code_location) : mafObject(code_location), m_CollectionMap(NULL), m_Interpolator(NULL), m_DataTypeAccepted(""), m_CurrentTimestamp(0.0) {
    m_CollectionMap = new mafDataSetMap();
    setInterpolator("mafResources::mafInterpolatorBefore");
}

mafDataSetCollection::~mafDataSetCollection() {
    mafDataSetMap::iterator iter = m_CollectionMap->begin();
    while(iter != m_CollectionMap->end()) {
        mafDEL(iter.value());
        ++iter;
    }
    emit(modifiedObject());

    m_CollectionMap->clear();
    delete m_CollectionMap;
    m_CollectionMap = NULL;
    
    mafDEL(m_Interpolator);
    
}

void mafDataSetCollection::setTimestamp(double t) {
    if(t != m_CurrentTimestamp) {
        m_CurrentTimestamp = t;
    }
}

void mafDataSetCollection::setInterpolator(mafInterpolator *interpolator) {
    REQUIRE(interpolator != NULL);

    mafDEL(m_Interpolator);
    m_Interpolator = interpolator;
}

void mafDataSetCollection::setInterpolator(const QString &interpolator_type) {
    mafObjectBase *obj = mafNEWFromString(interpolator_type);
    mafInterpolator *new_interpolator = qobject_cast<mafInterpolator *>(obj);
    if(new_interpolator) {
        setInterpolator(new_interpolator);
    } else {
        qWarning("%s", mafTr("%1 does not represent a type of mafInterpolator.").arg(interpolator_type).toAscii().data());
    }
}

void mafDataSetCollection::setPose(double rx, double ry, double rz, double x, double y, double z, double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = poseMatrix(t);
    if(m == NULL) {
        return;
    }
    // Write the orientation into the found matrix.
    writeOrientation(rx, ry, rz, m);
    // Write the position vector into the found matrix.
    writePosition(x, y, z, m);
}

void mafDataSetCollection::setOrientation(double rx, double ry, double rz, double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = poseMatrix(t);
    if(m == NULL) {
        return;
    }
    // Write the orientation into the found matrix.
    writeOrientation(rx, ry, rz, m);
}

void mafDataSetCollection::orientations(double ori[3], double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = poseMatrix(t);
    if(m == NULL) {
        return;
    }

    double nx, ny, nz;
    double ax, ay;
    double sx, sy;

    // Extract the rotation sub-matrix and calculate the angles considering the Yaw-Pitch-Roll convention.
    nx = m->get(0,0);
    ny = m->get(1,0);
    nz = m->get(2,0);
    ax = m->get(0,2);
    ay = m->get(1,2);
    sx = m->get(0,1);
    sy = m->get(1,1);

    ori[Z_AXIS] = atan2(ny, nx);
    ori[Y_AXIS] = atan2(-nz, cos(ori[Z_AXIS])*nx + sin(ori[Z_AXIS])*ny);
    ori[X_AXIS] = atan2(sin(ori[Z_AXIS])*ax-cos(ori[Z_AXIS])*ay, -sin(ori[Z_AXIS])*sx+cos(ori[Z_AXIS])*sy);

    ori[Z_AXIS] = radiantToDegrees(ori[Z_AXIS]);
    ori[Y_AXIS] = radiantToDegrees(ori[Y_AXIS]);
    ori[X_AXIS] = radiantToDegrees(ori[X_AXIS]);
}

void mafDataSetCollection::setPosition(double pos[3], double t) {
    setPosition(pos[0], pos[1], pos[2], t);
}

void mafDataSetCollection::setPosition(double x, double y, double z, double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = poseMatrix(t);
    if(m == NULL) {
        return;
    }
    // Write the position vector into the found matrix.
    writePosition(x, y, z, m);
}

void mafDataSetCollection::position(double pos[3], double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = poseMatrix(t);
    if(m == NULL) {
        return;
    }

    // Extract the position vector from the matrix and write it into the array.
    for(int i = 0; i < 3; ++i) {
        pos[i] = m->get(i,3);
    }
}

void mafDataSetCollection::setPose(const mafPoseMatrix matrix, double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = poseMatrix(t);
    if(m == NULL) {
        return;
    }
    *m = matrix;
}

mafPoseMatrix *mafDataSetCollection::poseMatrix(double t) {
    // Find the matrix at the given timestamp
    mafPoseMatrix *m = NULL;
    mafDataSet *item = itemAt(t);
    if(item != NULL) {
        m = item->poseMatrix();
    } else {
        qWarning("%s", mafTr("Item at timestamp %1 does not exist! Try 'insertItem' before.").arg(t).toAscii().data());
    }
    return m;
}

void mafDataSetCollection::writePosition(double x, double y, double z, mafPoseMatrix *m) {
    // Write the position vector into the given matrix.
    m->put(0,3, x);
    m->put(1,3, y);
    m->put(2,3, z);
}

void mafDataSetCollection::writeOrientation(double rx, double ry, double rz, mafPoseMatrix *m) {
    // calculate the rotation sub-matrix considering the Yaw-Pitch-Roll convention.
    mafPoseMatrix Rz;
    mafPoseMatrix Ry;
    mafPoseMatrix Rx;

    Rz.set_identity();
    Ry.set_identity();
    Rx.set_identity();

    double rx_rad, ry_rad, rz_rad;
    rx_rad = degreesToRadiant(rx);
    ry_rad = degreesToRadiant(ry);
    rz_rad = degreesToRadiant(rz);

    Rz.put(0,0,cos(rz_rad));
    Rz.put(1,1,cos(rz_rad));
    Rz.put(0,1,-sin(rz_rad));
    Rz.put(1,0,sin(rz_rad));

    Ry.put(0,0,cos(ry_rad));
    Ry.put(2,2,cos(ry_rad));
    Ry.put(0,2,sin(ry_rad));
    Ry.put(2,0,-sin(ry_rad));

    Rx.put(1,1,cos(rx_rad));
    Rx.put(2,2,cos(rx_rad));
    Rx.put(1,2,-sin(rx_rad));
    Rx.put(2,1,sin(rx_rad));

    // Store the old position for the matrix m
    double pos[3];
    for(int i = 0; i < 3; ++i) {
        pos[i] = m->get(i,3);
    }

    // Copy into 'm' the result of the matrix multiplication.
    *m = Rz * Ry * Rx;
    // Re-Apply the position
    writePosition(pos[0], pos[1], pos[2], m);
}

bool mafDataSetCollection::insertItem(mafDataSet *item, double t) {
    REQUIRE(item != NULL);

    double ts = t<0 ? m_CurrentTimestamp : t;

    //TODO: move time information from vme to dataSet
    //item->setTimeStamp(ts);

    // If the timestamp is present, no new insert is needed, just
    // modify the old one.
    if(m_CollectionMap->contains(ts)) {
        return setDataSet(item, ts);
    }

    // New timestamp; need to insert it into the collection if data value is acceptable.
    bool result(true);
    mafProxyInterface *data_value = item->dataValue();
    if(data_value != NULL) {
        result = acceptData(item);
    }

    if(result) {
        m_CollectionMap->insert(ts, item);
        item->retain();
        
        emit(modifiedObject());
    }

    return result;
}

bool mafDataSetCollection::setDataSet(mafDataSet *data, double t) {
    REQUIRE(data != NULL);

    if(acceptData(data)) {
        // Find the matrix at the given timestamp
        mafDataSet *item = itemAt(t);
        if(item != NULL) {
            // Item found into the collection.
            // Modify the already present item and free the memory
            item->setDataValue(data->dataValue());
            item->setPoseMatrix(data->poseMatrix());
            mafDEL(data);

            //check where to emit!!!
            emit(modifiedObject());


            return true;
        } else {
            qWarning("%s", mafTr("Item at timestamp %1 can not be modified, because it doesn't exist! Try 'insertItem' before").arg(t).toAscii().data());
            return false;
        }
    }
    qWarning("%s", mafTr("Item at timestamp %1 can not be accepted! Acceptable type is %2").arg(QString::number(t), m_DataTypeAccepted).toAscii().data());
    return false;
}

void mafDataSetCollection::updateData() {
  emit(modifiedObject());
}

bool mafDataSetCollection::acceptData(mafDataSet *data) {
    REQUIRE(data != NULL);

    bool result(true);
    mafProxyInterface *new_data_value = data->dataValue();
    QString new_data_type = new_data_value->externalDataType();
    if(m_DataTypeAccepted == "") {
        // the collection doesn't contain yet any data; initialize the acceptable
        // data type with that one contained in the current mafDataSet.
        m_DataTypeAccepted = new_data_type;
    } else {
        // if the collection contains data and now is inserting a dataset with
        // only a pose matrix, the data is again acceptable.
        result = new_data_type.isEmpty() ? true : m_DataTypeAccepted == new_data_type;
    }
    return result;
}

mafDataSet *mafDataSetCollection::itemAt(double t) {
    double ts = t<0 ? m_CurrentTimestamp : t;
    mafDataSet *item = m_Interpolator->itemAt(m_CollectionMap, ts);
    if(item == NULL && m_CollectionMap->size() == 0) {
        // The collection is empty; this is the first item and can be added a default one.
        item = mafNEW(mafResources::mafDataSet);
        mafPoseMatrix *m;
        m = new mafPoseMatrix();
        m->set_identity();
        item->setPoseMatrix(m);
        insertItem(item, ts);
        item->release();
        delete m;
        m = NULL;
    }
    return item;
}

mafDataSet *mafDataSetCollection::itemAtCurrentTime() {
    return itemAt(m_CurrentTimestamp);
}

bool mafDataSetCollection::removeItem(mafDataSet *item, bool keep_alive) {
    REQUIRE(item != NULL);

    // Method below can be slow for big map. QMap is optimized for finding value starting from key and not viceversa.
    double timestamp = m_CollectionMap->key(item, -1.0);
    if(timestamp != -1) {
        int removed_items = m_CollectionMap->remove(timestamp);
        emit(modifiedObject());
        if(!keep_alive) {
            mafDEL(item);
        }
        return removed_items == 1;
    }
    return false;
}

void mafDataSetCollection::itemDestroyed() {
    mafDataSet *item = (mafDataSet *)QObject::sender();
    removeItem(item, true);
}

mafMemento *mafDataSetCollection::createMemento() const {
  return new mafMementoDataSetCollection(this, mafCodeLocation);
}

void mafDataSetCollection::setMemento(mafMemento *memento, bool deep_memento) {
  Q_UNUSED(deep_memento);

  // Design by contract condition.
  REQUIRE(memento != NULL);
  REQUIRE(memento->objectClassType() == this->metaObject()->className());

  mafMementoPropertyList *list = memento->mementoPropertyList();
  mafMementoPropertyItem item;

  QList<double> timeList;
  foreach(item, *list) {
    if(item.m_Name == "timeStamp") {
      //Fill the collection with timeStamps. Later will be filled with data set.
      timeList.append(item.m_Value.toDouble());
    }
  }

  int timeIndex = 0;
  int n = 0;
  int childrenNum = memento->children().size();
  for (n; n < childrenNum; n++) {
    mafMemento *m = (mafMemento *)memento->children().at(n);
    if (m->serializationPattern() == mafSerializationPatternInheritance) {
      //set the memento of the superclass
      Superclass::setMemento(m, deep_memento);
    } else {
      //set the memento of the children memento
      QString objClassType = m->objectClassType();
      mafCore::mafObjectBase *objBase = mafNEWFromString(objClassType);
      mafCore::mafObject *obj = qobject_cast<mafCore::mafObject *>(objBase);
      obj->setMemento(m, deep_memento);
      if (objClassType == "mafResources::mafDataSet") {
        mafDataSet *data = qobject_cast<mafDataSet*>(obj);
        //Fill collection with dataSet. Time stamp will be set later, reading the memento property list.
        m_CollectionMap->insert(timeIndex, data);
        timeIndex++;
      }
    }
  }
}
