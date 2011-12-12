/*
 *  mafUILoader.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 29/10/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFUILOADER_H
#define MAFUILOADER_H

// Includes list
#include "mafGUIDefinitions.h"
#include <mafObjectBase.h>
#include <mafProxyInterface.h>

namespace mafGUI {
/**
  Class Name: mafUILoader
  UILoader represents the interface class for loading graphical user interface from
  file (generally XML). It parse XML and instantiates at runtime graphical objects which
  can be attached to panels, windows etc...
  */
class MAFGUISHARED_EXPORT mafUILoader : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

Q_SIGNALS:
    /// signal for inform that the user interface has been loaded from file
    void uiLoadedSignal(mafCore::mafProxyInterface *guiWidget, int ui_type);

public:
    /// Object constructor.
    mafUILoader(const QString code_location = "");

    /// Load from external file (generally an XML) the GUI for the MAF object type 'ui_type' (eg. VME, View...). GUI types are defined into mafGUIDefinitions.h
    virtual void uiLoad(const QString &fileName, int ui_type) = 0;

protected:
    /// Object destructor.
    /* virtual */ ~mafUILoader();
};

} //end namespace mafGUI

#endif // MAFUILOADER_H
