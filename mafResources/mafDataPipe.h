/*
 *  mafDataPipe.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPE_H
#define MAFDATAPIPE_H

// Includes list
#include "mafResourcesDefinitions.h"
#include "mafPipe.h"


namespace mafResources {

// Class forwarding list

/**
 Class name: mafDataPipe
 This is the base class for the MAF3 data pipes.
 */
class MAFRESOURCESSHARED_EXPORT mafDataPipe : public mafPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

public:
    /// Object constructor.
    mafDataPipe(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafDataPipe();

    /// Allow to add mafDataPipe to extend the elaboration following the pattern decorator.
    /** This method allows to compose data pipes funcionalities according to the decorator pattern and
    return the pointer to the decorator pipe, so you can add another decorator pipe in cascade.*/
    mafDataPipe *decorateWithDataPipe(mafDataPipe *pipe);

    /// Get output of the pipe.
    mafDataSet *output();

protected:
    mafDataPipe *m_DecoratorPipe; ///< Pointer to the decorator data pipe class.
    mafDataSet *m_Output; ///< Output Data.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafDataPipe *mafDataPipe::decorateWithDataPipe(mafDataPipe *pipe) {
    m_DecoratorPipe = pipe;
    return m_DecoratorPipe;
}

inline mafDataSet *mafDataPipe::output() {
    return m_Output;
}

} //namespace mafResources

#endif // MAFDATAPIPE_H
