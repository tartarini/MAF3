/*
 *  mafUILoaderQt.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 29/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafUILoaderQt.h"
#include <mafContainer.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

mafUILoaderQt::mafUILoaderQt(const QString code_location) : mafUILoader(code_location) {
}

mafUILoaderQt::~mafUILoaderQt() {
}

void mafUILoaderQt::uiLoad(const QString &fileName) {
    QFile file(fileName.toAscii());
    file.open(QFile::ReadOnly);
    mafContainer<QWidget> *arg = new mafContainer<QWidget>();
    *arg = m_UILoaderQt.load(&file);
    file.close();

    mafContainerInterface *gui = arg;
    mafEventArgumentsList list;
    list.append(mafEventArgument(mafCore::mafContainerInterface *, gui));
    mafEventBusManager::instance()->notifyEvent("maf.local.gui.uiloaded", mafEventTypeLocal, &list);
}
