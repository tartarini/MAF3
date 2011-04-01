/*
 *  mafUILoaderQt.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 29/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFUILOADERQT_H
#define MAFUILOADERQT_H

#include "mafUILoader.h"

namespace mafGUI {

/**
  Class Name: mafUILoaderQt
  It represents the specific class for loading Qt graphical user interface from
  .ui file  It parse xml and instantiates at runtime graphical Qt widgets.
  */
class MAFGUISHARED_EXPORT mafUILoaderQt : public mafUILoader {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafGUI::mafUILoader);

public:
    /// Object constructor.
    mafUILoaderQt(const QString code_location = "");

    /// load from external file (generally an xml) the GUI
    /*virtual*/ void uiLoad(const QString &fileName);

protected:
    /// Object destructor.
    /* virtual */ ~mafUILoaderQt();

private:
    QUiLoader m_UILoaderQt;///< instance of the .ui file loader
};

} //end namespace mafGUI

#endif // MAFUILOADERQT_H
