/*
 *  mafUILoaderQt.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 29/10/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFUILOADERQT_H
#define MAFUILOADERQT_H

#include "mafUILoader.h"

namespace mafGUI {

/**
  Class Name: mafUILoaderQt
  It represents the specific class for loading Qt graphical user interface from
  .ui file  It parse XML and instantiates at runtime graphical Qt widgets.
  */
class MAFGUISHARED_EXPORT mafUILoaderQt : public mafUILoader {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafGUI::mafUILoader);

public:
    /// Object constructor.
    mafUILoaderQt(const QString code_location = "");

    /// Load from external file (generally an XML) the GUI
    /*virtual*/ void uiLoad(const QString &fileName, int ui_type);

protected:
    /// Object destructor.
    /* virtual */ ~mafUILoaderQt();

private:
    QUiLoader m_UILoaderQt;///< instance of the .ui file loader
};

} //end namespace mafGUI

#endif // MAFUILOADERQT_H
