/*
 *  mafVisualPipe.h
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
 Class name: mafVisualPipe
 This is the base class for the MAF3 visual pipes.
 */
class MAFRESOURCESSHARED_EXPORT mafVisualPipe : public mafPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

public:
     /// Object constructor.
    mafVisualPipe(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafVisualPipe();

    /// Get output of the pipe.
    mafCore::mafContainerInterface *output();

signals:
    void vmeSelectSignal(double *selectPos, unsigned long,  mafCore::mafContainerInterface *actor);


private slots:
    void vmeSelect(double *selectPos, unsigned long, mafCore::mafContainerInterface *actor);

private:
    void initializeConnections();


protected:
    mafCore::mafContainerInterface *m_Output; ///< Output for visual pipe.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafContainerInterface *mafVisualPipe::output() {
    return m_Output;
}

} //namespace mafResources

#endif // MAFVISUALPIPE_H
