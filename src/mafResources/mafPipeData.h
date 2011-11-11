/*
 *  mafPipeData.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
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
 Class name: mafPipeData
 This is the base class for the MAF3 data pipes.
 */
class MAFRESOURCESSHARED_EXPORT mafPipeData : public mafPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

protected Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /** By default it simply initialize the output with the first element in the input list.*/
    /*virtual*/ void updatePipe(double t = -1);

public:
    /// Object constructor.
    mafPipeData(const QString code_location = "");

    /// Append a new VME to the input list.
    /** This method allows to add a VME to the input list. It returns the index of the last VME added.*/
    int addInput(mafVME *vme);

    /// Allow to add mafPipeData to extend the elaboration following the pattern decorator.
    /** This method allows to compose data pipes functionalities according to the decorator pattern and
    return the pointer to the decorator pipe, so you can add another decorator pipe in cascade.*/
    void decorateWithDataPipe(mafPipeData *pipe);

    /// Set to false, to create a new mafVME as output of the dataPipe
    void setWorksInPlace(bool inPlace);

    /// Get output of the pipe at a specific time
    mafVME *output(double t = -1);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeData();

private:
    mafPipeData *m_DecoratorPipe; ///< Pointer to the decorator data pipe class.
    mafVME *m_Output; ///< Output Data.
    bool m_InPlace; ///< If false, dataPipe creates a new mafVME as output.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafPipeData::decorateWithDataPipe(mafPipeData *pipe) {
    pipe->setInput(this->output());
    m_DecoratorPipe = pipe;
    m_DecoratorPipe->retain();
    setModified();
}

inline void mafPipeData::setWorksInPlace(bool inPlace) {
    m_InPlace = inPlace;
}

} //namespace mafResources

#endif // MAFDATAPIPE_H
