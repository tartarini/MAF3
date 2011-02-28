/*
 *  mafObjectRegistry.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOBJECTREGISTRY_H
#define MAFOBJECTREGISTRY_H

// Includes list
#include "mafCore_global.h"

namespace mafCore {

// Class forwarding list
class mafObjectBase;
class mafVisitorFindObjects;

/**
 Class name: mafObjectRegistry
 This singletone provides the place where allocated objects are monitored.
 In this way you can monitor when, where and who allocated the object.
 */
class MAFCORESHARED_EXPORT mafObjectRegistry  {
public:
    /// Return an instance of the registry.
    static mafObjectRegistry *instance();

    /// Dump into default logger all live objects still allocated and not deleted.
    void dumpLiveObjects();

    /// Fill the 'objects' list with all the live objects contained by the registry.
    void liveObjects(mafObjectsList *objects);

    /// Return the object pointer from its hash code.
    /** This is an example that demostrate the usage of findObjectsThreaded.
    It is also a shortcut for searching object by a given hash string. Non valid hash code will make return a NULL pointer.*/
    mafObjectBase *objectFromHash(const QString &hash);

    /// Find objects according to the given mafVisitor strategy. The returned value is a list of found objects.
    mafObjectsList *findObjectsThreaded(mafVisitorFindObjects *v);

    /// Engine of the 'find objects' that will be executed on separate thread if invoked from the findObjectsThreaded method.
    mafObjectsList *findObjects(mafVisitorFindObjects *v);

protected:
    friend class mafObjectBase;

    /// Register allocated object into the registry hash.
    void addObject(mafObjectBase *obj, const QString location);

    /// Remove object from the registry hash. It appens when object is deleted.
    void removeObject(mafId obj_id);

private:
    /// Object constructor.
    mafObjectRegistry() {}

    /// Object destructor.
    ~mafObjectRegistry();

    /** struct that contain information on allocated object.*/
    struct mafObjectRegistryItem {
        mafObjectBase *m_Object;
        QString m_InstantiateLocationInfo;
        QTime m_AllocationTime;
        int m_ReferenceCount;
    };

    typedef QHash<mafId, mafObjectRegistryItem> mafRegistryHashType;
    mafRegistryHashType m_Registry; ///< Object allocation hash.
};

} // mafCore

#endif // MAFOBJECTREGISTRY_H
