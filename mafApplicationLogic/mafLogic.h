/*
 *  mafLogic.h
 *  mafApplicationLogic
 *
 *  Created by Daniele Giunchi on 28/06/10.
 *  Copyright 2009-2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGIC_H
#define MAFLOGIC_H

// Includes list
#include "mafLogicLight.h"

namespace mafApplicationLogic {

// Class forwarding list

/**
 Class name: mafLogic
 This class define the full implementation of the framework logic.
 It allows to instantiate all the managers, and use initialization of mafLogicLight.
 This logic class can be used with those applications that needs of a complete version of the framework
 using plugin and resources.
 mafLogic defines also the topic:
 - maf.local.logic.settings.store that allows to ask the observer to store their settings into a memento that will be serialized.
 - maf.local.logic.settings.restore that allows to restore a previous saved state of the application's settings.
 */
class MAFAPPLICATIONLOGICSHARED_EXPORT mafLogic : public mafLogicLight {
    Q_OBJECT
    Q_PROPERTY(QString workingDir READ workingDirectory WRITE setWorkingDirectory);
    /// typedef macro.
    mafSuperclassMacro(mafLogic::mafLogicLight);

public:
    /// Object constructor.
    mafLogic(const mafString code_location = "");

    /// Object destructor.
    /*virtual*/ ~mafLogic();

    /// Allows to initialize all the singletons and register all the objects to the factory.
    virtual void initialize();

    /// Allows to assign the application's working directory.
    void setWorkingDirectory(const mafString wd);

    /// Allows to retrieve the application's working directory.
    mafString workingDirectory() const;

private:
    /// Allow to load plugins located into the 'plugins' folder located into the application's directory path.
    void loadPlugins();

    mafString m_WorkingDirectory; ///< Contains the root directory of the application's data.
    mafString m_ApplicationDirectory; ///< Contains the application's working directory.
};

inline void mafLogic::setWorkingDirectory(const mafString wd) {
    m_WorkingDirectory = wd;
}

inline mafString mafLogic::workingDirectory() const {
    return m_WorkingDirectory;
}

} // namespace mafApplicationLogic

#endif // MAFLOGIC_H
