/*
 *  mafCoreSingletons.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreSingletons.h"
#include "mafCoreRegistration.h"


using namespace mafCore;

void mafCoreSingletons::mafSingletonsInitialize() {
    mafObjectRegistry::instance();
    mafIdProvider::instance();
    mafObjectFactory::instance();
    mafThreadSynchronizer::instance();
    mafMessageHandler::instance()->installMessageHandler();
    mafCoreRegistration::registerCoreObjects();
}

void mafCoreSingletons::mafSingletonsShutdown() {
    mafMessageHandler::instance()->shutdown();
    mafObjectFactory::instance()->shutdown();
    mafIdProvider::instance()->shutdown();
    mafThreadSynchronizer::instance()->shutdown();
}

QLibrary *mafInitializeModule(QString module_library) {
    typedef void mafFnInitModule();
    mafFnInitModule *initModule;

    QLibrary *libraryHandler;

    libraryHandler = new QLibrary(module_library);
    if(!libraryHandler->load()) {
        QString err_msg(mafTr("Could not load '%1'").arg(module_library));
        qCritical("%s", err_msg.toAscii().constData());
        return false;
    }

    // Get the handle to the 'initializeModule' function
    initModule = reinterpret_cast<mafFnInitModule *>(libraryHandler->resolve("initializeModule"));
    if(!initModule) {
        QString err_msg(mafTr("'%1' module can not be initialized!!").arg(module_library));
        qCritical("%s", err_msg.toAscii().constData());
        return NULL;
    }

    // ...and if no errors occourred, call the module initialization.
    initModule();
    return libraryHandler;
}

bool mafShutdownModule(QLibrary *libraryHandler) {
    REQUIRE(libraryHandler);

    typedef void mafFnShutdownModule();
    mafFnShutdownModule *shutdownModule;

    // Get the handle to the 'initializeModule' function
    shutdownModule = reinterpret_cast<mafFnShutdownModule *>(libraryHandler->resolve("shutdownModule"));
    if(!shutdownModule) {
        QString err_msg(mafTr("'%1' module can not shutdown!!").arg(libraryHandler->fileName()));
        qCritical("%s", err_msg.toAscii().constData());
        return false;
    }

    // ...and if no errors occourred, call the module shutdown.
    shutdownModule();

    // and then unload the library.
    return libraryHandler->unload();
}
