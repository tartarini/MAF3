/*
 *  mafPipeVisual.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPE_H
#define MAFVISUALPIPE_H

// Includes list
#include "mafPipe.h"
#include <mafContainerInterface.h>

namespace mafResources {

// Class forwarding list

/**
 Class name: mafPipeVisual
 This is the base class for the MAF3 visual pipes. The class defines the topic used to pick VMEs:
 - maf.local.resources.interaction.vmePick
 */
class MAFRESOURCESSHARED_EXPORT mafPipeVisual : public mafPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

public:
     /// Object constructor.
    mafPipeVisual(const QString code_location = "");

    /// Get output of the pipe.
    mafCore::mafContainerInterface *output();

    /// Return the visibility status
    bool visibility() const;

signals:
    /// Signal emitted when the pick hits the owned object.
    void vmePickSignal(double *pickPos, unsigned long modifiers,  mafCore::mafContainerInterface *actor, QEvent *);

    ///
    void vmePickedSignal(double *pickPos, unsigned long modifiers , mafCore::mafObjectBase *vme);

public slots:
    /// Set the visibility of its rendering scene.
    virtual void setVisibility(bool visible);

private slots:
    /// Forward the vmePick event if the pick hits the current visualized VME.
    void vmePick(double *pickPos, unsigned long, mafCore::mafContainerInterface *actor, QEvent *);

private:
    /// Register signals and slots connections with the event bus.
    void initializeConnections();

    bool m_Visibility; ///< Contains the visibility status of the owned object/s

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisual();

    mafCore::mafContainerInterface *m_Output; ///< Output for visual pipe.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafContainerInterface *mafPipeVisual::output() {
    return m_Output;
}

inline bool mafPipeVisual::visibility() const {
    return m_Visibility;
}

} //namespace mafResources

#endif // MAFVISUALPIPE_H
