/*
 *  mafPipe.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPE_H
#define MAFPIPE_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list

/**
 Class name: mafPipe
 This is the base class for the MAF3  pipes.
 */
class MAFRESOURCESSHARED_EXPORT mafPipe : public mafCore::mafObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
     /// Object constructor.
    mafPipe(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafPipe();

    /// Set the input dataset to be used as input data for the pipeline.
    /** This method allows to assign a dataset to the input list. It returns the index of the last dataset added.
    If something goes wrong -1 is returned. If a dataset is already present into the input list, it is substituted by the new one.*/
    int setInput(mafDataSet *data, const int idx = 0);

    /// Assign an multi-input to the mafPipe.
    /** This method allows to add a collection of datasets and return the index of the last dataset added.
    If something goes wrong -1 is returned.*/
    virtual int addInputMulti(mafDataSetCollection *data);

    /// Append a new data to the input list.
    /** This method allows to add a dataset to the input list. It returns the index of the last dataset added.
    If something goes wrong -1 is returned.*/
    int addInput(mafDataSet *data);

    /// Remove the input at index 'idx'
    void removeInput(const int idx);

    /// Remove the the given input object from the list
    void removeInput(mafDataSet *data);

    /// Get the input list of the pipe.
    mafDataSetList *inputList();

    /// Initialize and create the pipeline
    virtual void createPipe() = 0;

public slots:
    /// Allow to execute and update the pipeline when something change
    virtual void updatePipe(double t = -1) = 0;

private slots:
    /// Method called when an input mafDataSet has been destroyed (by someone else).
    void inputDestroyed();

private:
    mafDataSetList *m_InputList; ///< Data input List
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafDataSetList *mafPipe::inputList() {
    return m_InputList;
}

} //namespace mafResources

#endif // MAFPIPE_H
