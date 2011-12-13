/*
 *  mafViewCompoundConfigurator.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWCOMPOUNDCONFIGURATOR_H
#define MAFVIEWCOMPOUNDCONFIGURATOR_H

// Include list
#include "mafGUIDefinitions.h"
#include <QDomDocument>

// Class forwarding
class mafSplitter;

namespace mafGUI {

/**
 Class name: mafViewCompoundConfigurator
 This class is the object that is in charge to parse the input XML file and create a compound view configuration.
 */
class MAFGUISHARED_EXPORT mafViewCompoundConfigurator {
public:
    /// Object constructor
    mafViewCompoundConfigurator();

    /// Object destructor
    ~mafViewCompoundConfigurator();

    /// Read the configuration file given as argument and return true on success.
    bool parseConfigurationFile(QString configFile);

    /// Return the root object of the compound widget
    QObject *rootObject();

private:
    /// Allow to parse the XML configuration file document.
    void parseDocument(QDomNode current, QObject *parent);

    mafSplitter *m_RootObject; ///< Root splitter of the compound widget.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QObject *mafViewCompoundConfigurator::rootObject() {
    return (QObject *)m_RootObject;
}

} // namespace mafGUI

#endif // MAFVIEWCOMPOUNDCONFIGURATOR_H
