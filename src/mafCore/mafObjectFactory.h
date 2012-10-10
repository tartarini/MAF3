/*
 *  mafObjectFactory.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFOBJECTFACTORY_H
#define MAFOBJECTFACTORY_H

// Includes list
#include "mafClassFactory.h"
#include "mafQtClassFactory.h"
#include "mafObjectBase.h"
#include "mafSmartPointer.h"
#include "mafObjectRegistry.h"

#define mafRegisterObject(maf_object_type) \
    mafCore::mafObjectFactory::instance()->registerObject<maf_object_type>(#maf_object_type);

#define mafRegisterQtObject(qt_object_type) \
    mafCore::mafObjectFactory::instance()->registerQtObject<qt_object_type>(#qt_object_type);

#define mafUnregisterObject(maf_object_type) \
    mafCore::mafObjectFactory::instance()->unregisterObject(#maf_object_type);

#define mafUnregisterQtObject(qt_object_type) \
    mafCore::mafObjectFactory::instance()->unregisterQtObject(#qt_object_type);

#define mafNEW(maf_object_type) \
    mafCore::mafObjectFactory::instance()->instantiateObject<maf_object_type>(#maf_object_type, mafCodeLocation);

#define mafNEWQt(qt_object_type) \
    mafCore::mafObjectFactory::instance()->instantiateQtObject<qt_object_type>(#qt_object_type, mafCodeLocation);

#define mafNEWFromString(maf_object_type_string) \
    mafCore::mafObjectFactory::instance()->instantiateObjectBase(maf_object_type_string, mafCodeLocation);

#define mafNEWQtFromString(qt_object_type_string) \
    mafCore::mafObjectFactory::instance()->instantiateQtObjectFromString(qt_object_type_string, mafCodeLocation);

#define mafDEL(object_pointer) \
    if(object_pointer != NULL) { \
        bool rc = object_pointer->referenceCount() == 1; \
        object_pointer->release(); \
        if(rc) { \
            object_pointer = NULL; \
        } \
    }

#ifdef WIN32
#define mafDELThread(thread_pointer) \
    if(thread_pointer != NULL) { \
        thread_pointer->exit(0); \
        delete thread_pointer; \
        thread_pointer = NULL; \
    }
#else
#define mafDELThread(thread_pointer) \
    if(thread_pointer != NULL) { \
        thread_pointer->exit(0); \
        thread_pointer->deleteLater(); \
    }
#endif

#define mafCreateSmartObject(maf_object_type) \
    mafCore::mafObjectFactory::instance()->instantiateSmartObject<maf_object_type>(#maf_object_type);

#define mafBindObjectToIcon(objectType, iconFileName) \
    mafCore::mafObjectFactory::instance()->bindObjectToIcon(objectType, iconFileName);

#define mafIconFromObjectType(objectType) \
    mafCore::mafObjectFactory::instance()->iconFromObjectType(objectType);

namespace mafCore {

/**
 Class name: mafObjectFactory
 This class represent the factory for all the MAF3 objects.
 Objects are instantiated by calling the static method CreateObject and passing as argument the typename of the object to create.
 */
class mafObjectFactory {
public:
    /// Return an instance of the mafObjectFactory.*/
    MAFCORESHARED_EXPORT static mafObjectFactory *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    MAFCORESHARED_EXPORT void shutdown();
    
    /// Allows to register MAF objects into the factory to be created.
    template<typename T> void registerObject( const QString &className );

    /// Allows to register Qt objects into the factory to be created.
    template<typename T> void registerQtObject( const QString &className );
    
    /// Allows to unregister MAF objects from the factory.
    MAFCORESHARED_EXPORT bool unregisterObject( const QString &className );

    /// Allows to unregister Qt objects from the factory.
    MAFCORESHARED_EXPORT bool unregisterQtObject( const QString &className );

    /// check if a MAF object has been registered into the factory.
    MAFCORESHARED_EXPORT bool isObjectRegistered( const QString &className );

    /// check if a Qt object has been registered into the factory.
    MAFCORESHARED_EXPORT bool isQtObjectRegistered( const QString &className );
    
    /// Create an instance of the object given its typename as a string.
    /** This function differs from the instantiateObjectBase because it returns the allocated object
    casted to its real type, so the function is template and has to be called with the real type.*/
    template<typename T> T* instantiateObject( const QString& className, const QString location = "");

    /// Create an instance of the Qt object given its typename as a string.
    /** This function differs from the instantiateObjectBase because it returns the allocated object
    casted to its real type, so the function is template and has to be called with the real type.*/
    template<typename T> T* instantiateQtObject( const QString& className, const QString location = "");

    /// Create an instance of the object given its typename as string.
    /** This function allocate the object given its typename as string (considering the namespace) and return
    the pointer of the mafObjectBase which is the base of all object in MAF. This method is used by the mafCodec
    classes and in all the other situation where is only a string and not the corresponding type definition.*/
    MAFCORESHARED_EXPORT mafObjectBase *instantiateObjectBase( const QString &className, const QString location);

    /// Create an instance of the object given its typename as string.
    /** This function allocate the object given its typename as string (considering the namespace) and return
    the pointer of the mafObjectBase which is the base of all object in MAF. This method is used by the mafCodec
    classes and in all the other situation where is only a string and not the corresponding type definition.*/
    MAFCORESHARED_EXPORT QObject *instantiateQtObjectFromString( const QString &className, const QString location);

    /// Allows creation of mafSmartPointer object given the class type of the object to allocate.
    template <typename T> mafSmartPointer<T> instantiateSmartObject( const QString& className );

    /// Allows to bind a MAF objects to an icon file name.
    MAFCORESHARED_EXPORT void bindObjectToIcon( const QString objectType, const QString iconFileName );

    /// Allows to unregister MAF objects from the factory.
    MAFCORESHARED_EXPORT QString iconFromObjectType( const QString objectType );


protected:
    
private:
    /// mafObjectFactory constructor.
    mafObjectFactory() {};
    
    /// mafObjectFactory destructor.
    ~mafObjectFactory();

    typedef QHash<QString, mafObjectFactoryInterface*> mafObjectFactoryMapType;
    mafObjectFactoryMapType m_ObjectMap; ///< Hash table used for storing MAF registered objects.

    typedef QHash<QString, mafQtObjectFactoryInterface*> mafQtObjectFactoryMapType;
    mafQtObjectFactoryMapType m_QtObjectMap; ///< Hash table used for storing Qt registered objects.

    typedef QHash<QString, QString> mafObjectIconHash;
    mafObjectIconHash m_ObjectIconHash; ///< Hash to associate objects type with an icon.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

template <typename T>
void mafObjectFactory::registerObject( const QString& className ) {
    if (!isObjectRegistered(className)) {
        m_ObjectMap.insert(className, new mafClassFactory<T>);
    }
}

template <typename T>
void mafObjectFactory::registerQtObject( const QString& className ) {
    if (!isQtObjectRegistered(className)) {
        m_QtObjectMap.insert(className, new mafQtClassFactory<T>);
    }
}

template <typename T>
T *mafObjectFactory::instantiateObject( const QString& className, const QString location ) {
    if (!isObjectRegistered(className)) {
        this->registerObject<T>(className);
    }
    QObject *obj = m_ObjectMap.value(className)->make(location);
    return qobject_cast<T *>(obj);
}

template <typename T>
T *mafObjectFactory::instantiateQtObject( const QString& className, const QString location ) {
     if (!isQtObjectRegistered(className)) {
         this->registerQtObject<T>(className);
     }
    QObject *obj = m_QtObjectMap.value(className)->make();
    //mafObjectRegistry::instance()->addObject(obj, location);
    return qobject_cast<T *>(obj);
}

template <typename T>
mafSmartPointer<T> mafObjectFactory::instantiateSmartObject( const QString& className ) {
    if (!isObjectRegistered(className)) {
        this->registerObject<T>(className);
    }
    QObject *obj = m_ObjectMap.value(className)->make();
    T *p = qobject_cast<T *>(obj);
    return mafSmartPointer<T>(p);
}


}

#endif  // MAFOBJECTFACTORY_H
